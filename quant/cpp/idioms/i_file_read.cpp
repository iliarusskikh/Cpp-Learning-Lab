/*
 * Quant Challenge (standalone — no other repo files needed):
 *
 * You have a 10 million row file formatted as {Location;Temperature}
 * called gas_stations.csv. There are 10'000 unique locations (stations).
 * Temperature readings are -99.9 to 99.9. All floats are single decimal
 * (e.g. 22.0). You want to read it quickly and compute summary statistics
 * (min, max, average). What should you do?
 *
 * OPTIMISATION STRATEGY (why this is fast)
 * ----------------------------------------
 * 1. Avoid extra buffers
 *    - Prefer mmap (real file) or one contiguous byte buffer (synthetic).
 *    - No getline / vector-of-lines / per-row std::string copies.
 *    - Station names stay as (ptr,len) into that buffer (zero-copy keys).
 *
 * 2. Never call stof / stod / atof / stringstream
 *    - Float parsing is slow (locale, branching, heap strings).
 *    - Temps have a fixed shape: optional '-', 1–2 digits, '.', one digit.
 *    - Parse as fixed-point tenths of a degree into int16_t
 *      e.g. "22.0" -> 220, "-9.5" -> -95.  Range fits in int16 (-999..999).
 *
 * 3. Accumulate in integers, not floats
 *    - Stats hold: min, max, sum (int64), count (uint32).
 *    - Average = sum / (10.0 * count) only once, at print time.
 *    - Avoids float work in the 10M-row hot loop.
 *
 * 4. Custom open-addressing hash map (not std::unordered_map)
 *    - No per-insert heap nodes / string copies.
 *    - 10k stations -> table sized 32k (power of 2) for low load factor.
 *
 * 5. Hot loop is branch-light pointer walking
 *    - Find ';', hash/compare name, parse temp digits, advance past '\n'.
 *
 * Build:  g++ -O3 -std=c++17 -o qcode_fileread qcode_fileread.cpp
 * Run:    ./qcode_fileread              # synthetic in-memory data (demo)
 *         ./qcode_fileread data.csv     # mmap a real file if you have one
 */

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

// Challenge sizes (synthetic generator uses these; override via env if desired)
static constexpr uint32_t kNumStations = 10000;
static constexpr uint64_t kNumRows     = 10'000'000ull;

// ---------------------------------------------------------------------------
// Per-station accumulator (all integer until final report)
// ---------------------------------------------------------------------------
struct StationStats {
    int16_t  min =  999;   // tenths of a degree
    int16_t  max = -999;
    int64_t  sum = 0;      // sum of tenths
    uint32_t count = 0;

    void update(int16_t t) {
        if (t < min) min = t;
        if (t > max) max = t;
        sum += t;
        ++count;
    }
};

// ---------------------------------------------------------------------------
// Zero-copy key: points into the mapped / generated buffer
// ---------------------------------------------------------------------------
struct StationKey {
    const char* ptr = nullptr;
    uint32_t    len = 0;

    bool operator==(const StationKey& o) const {
        return len == o.len && std::memcmp(ptr, o.ptr, len) == 0;
    }
};

// FNV-1a 64-bit — cheap, good enough for short station names
static uint64_t hash_bytes(const char* p, uint32_t n) {
    uint64_t h = 14695981039346656037ull;
    for (uint32_t i = 0; i < n; ++i) {
        h ^= static_cast<uint8_t>(p[i]);
        h *= 1099511628211ull;
    }
    return h;
}

// ---------------------------------------------------------------------------
// Open-addressing hash map: StationKey -> StationStats
// Power-of-two capacity; linear probe. No heap per entry beyond the array.
// ---------------------------------------------------------------------------
class StationMap {
public:
    explicit StationMap(std::size_t capacity_pow2)
        : mask_(capacity_pow2 - 1),
          slots_(capacity_pow2) {}

    StationStats& get_or_insert(StationKey key) {
        uint64_t h = hash_bytes(key.ptr, key.len);
        std::size_t i = static_cast<std::size_t>(h) & mask_;
        for (;;) {
            Slot& s = slots_[i];
            if (!s.used) {
                s.used = true;
                s.key  = key;
                return s.stats;
            }
            if (s.key == key)
                return s.stats;
            i = (i + 1) & mask_;
        }
    }

    std::vector<std::pair<StationKey, StationStats>> entries() const {
        std::vector<std::pair<StationKey, StationStats>> out;
        out.reserve(kNumStations);
        for (const Slot& s : slots_) {
            if (s.used)
                out.emplace_back(s.key, s.stats);
        }
        return out;
    }

private:
    struct Slot {
        bool         used = false;
        StationKey   key;
        StationStats stats;
    };
    std::size_t       mask_;
    std::vector<Slot> slots_;
};

// ---------------------------------------------------------------------------
// Parse temperature in tenths. Forms: "X.X", "XX.X", "-X.X", "-XX.X"
// No floats, no library converters — only digit arithmetic.
// ---------------------------------------------------------------------------
static int16_t parse_temp_tenths(const char*& p) {
    int sign = 1;
    if (*p == '-') {
        sign = -1;
        ++p;
    }

    int v = *p++ - '0';
    if (*p != '.')
        v = v * 10 + (*p++ - '0');

    ++p; // skip '.'
    int frac = *p++ - '0';

    return static_cast<int16_t>(sign * (v * 10 + frac));
}

// ---------------------------------------------------------------------------
// Process contiguous {Location;Temperature\n} bytes end-to-end
// ---------------------------------------------------------------------------
static void process(const char* data, std::size_t size, StationMap& map) {
    const char* p   = data;
    const char* end = data + size;

    while (p < end) {
        const char* name = p;
        while (*p != ';')
            ++p;
        StationKey key{name, static_cast<uint32_t>(p - name)};
        ++p; // skip ';'

        int16_t t = parse_temp_tenths(p);
        map.get_or_insert(key).update(t);

        if (p < end && *p == '\r') ++p;
        if (p < end && *p == '\n') ++p;
    }
}

// ---------------------------------------------------------------------------
// Synthetic generator — same wire format as gas_stations.csv, no disk needed.
// Names are "S00000".."S09999" so they are fixed-width and easy to parse.
// ---------------------------------------------------------------------------
static std::string generate_synthetic(uint64_t rows, uint32_t stations) {
    // Rough upper bound: "S99999;-99.9\n" = 14 bytes
    std::string buf;
    buf.reserve(static_cast<std::size_t>(rows * 14));

    char name[8]; // "Sxxxxx" + NUL
    name[0] = 'S';

    // Simple LCG — no <random> overhead, deterministic
    uint64_t state = 0xC0FFEEULL;

    for (uint64_t i = 0; i < rows; ++i) {
        state = state * 6364136223846793005ull + 1;
        uint32_t sid = static_cast<uint32_t>(state >> 33) % stations;

        // Write station id as 5 zero-padded digits
        uint32_t x = sid;
        name[5] = static_cast<char>('0' + (x % 10)); x /= 10;
        name[4] = static_cast<char>('0' + (x % 10)); x /= 10;
        name[3] = static_cast<char>('0' + (x % 10)); x /= 10;
        name[2] = static_cast<char>('0' + (x % 10)); x /= 10;
        name[1] = static_cast<char>('0' + (x % 10));

        state = state * 6364136223846793005ull + 1;
        // Uniform tenths in [-999, 999]
        int16_t tenths = static_cast<int16_t>(
            static_cast<int32_t>(state % 1999) - 999);

        buf.append(name, 6);
        buf.push_back(';');

        if (tenths < 0) {
            buf.push_back('-');
            tenths = static_cast<int16_t>(-tenths);
        }
        int whole = tenths / 10;
        int frac  = tenths % 10;
        if (whole >= 10) {
            buf.push_back(static_cast<char>('0' + whole / 10));
            buf.push_back(static_cast<char>('0' + whole % 10));
        } else {
            buf.push_back(static_cast<char>('0' + whole));
        }
        buf.push_back('.');
        buf.push_back(static_cast<char>('0' + frac));
        buf.push_back('\n');
    }
    return buf;
}

static void print_results(const StationMap& map) {
    auto rows = map.entries();
    std::sort(rows.begin(), rows.end(),
              [](const auto& a, const auto& b) {
                  return std::string_view(a.first.ptr, a.first.len) <
                         std::string_view(b.first.ptr, b.first.len);
              });

    std::cout.setf(std::ios::fixed);
    std::cout.precision(1);
    for (const auto& [key, s] : rows) {
        double avg = static_cast<double>(s.sum) / (10.0 * s.count);
        std::cout.write(key.ptr, static_cast<std::streamsize>(key.len));
        std::cout << '='
                  << (s.min / 10.0) << '/'
                  << avg            << '/'
                  << (s.max / 10.0) << '\n';
    }
}

int main(int argc, char** argv) {
    StationMap map(/*capacity_pow2=*/1u << 15); // 32k slots for ~10k stations
    const char* data = nullptr;
    std::size_t size = 0;
    void* mapped = nullptr;
    std::string synthetic; // owns generated bytes when no file is passed

    const char* path = nullptr;
    bool do_print = false;
    for (int i = 1; i < argc; ++i) {
        std::string_view a(argv[i]);
        if (a == "--print")
            do_print = true;
        else if (!path)
            path = argv[i];
    }

    auto t0 = std::chrono::steady_clock::now();

    if (path) {
        // Real file path provided: mmap it (fastest I/O path).
        int fd = ::open(path, O_RDONLY);
        if (fd < 0) {
            std::perror(path);
            return 1;
        }
        struct stat st{};
        if (::fstat(fd, &st) != 0 || st.st_size == 0) {
            std::perror("fstat");
            ::close(fd);
            return 1;
        }
        size = static_cast<std::size_t>(st.st_size);
        mapped = ::mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
        ::close(fd);
        if (mapped == MAP_FAILED) {
            std::perror("mmap");
            return 1;
        }
        ::madvise(mapped, size, MADV_SEQUENTIAL);
        data = static_cast<const char*>(mapped);
        std::cerr << "mmap'd " << size << " bytes from " << path << '\n';
    } else {
        // No related files in the repo: generate the challenge dataset in-memory.
        std::cerr << "generating " << kNumRows << " rows, "
                  << kNumStations << " stations (in-memory)...\n";
        synthetic = generate_synthetic(kNumRows, kNumStations);
        data = synthetic.data();
        size = synthetic.size();
        std::cerr << "buffer size " << size << " bytes\n";
    }

    auto t1 = std::chrono::steady_clock::now();
    process(data, size, map);
    auto t2 = std::chrono::steady_clock::now();

    if (do_print)
        print_results(map);
    else
        std::cerr << "stations aggregated: " << map.entries().size()
                  << "  (pass --print to dump min/avg/max)\n";

    using ms = std::chrono::duration<double, std::milli>;
    std::cerr << "load/generate: " << ms(t1 - t0).count() << " ms\n"
              << "parse+agg:     " << ms(t2 - t1).count() << " ms\n";

    if (mapped && mapped != MAP_FAILED)
        ::munmap(mapped, size);
    return 0;
}
