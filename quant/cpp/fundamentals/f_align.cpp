#include <atomic>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <new>
#include <thread>

// ---------------------------------------------------------------------------
// Cache-line size constants (C++17, <new>). Fallback of 64 covers x86-64 and
// most ARM cores (Apple M-series is 128, so don't hardcode it blindly).
// ---------------------------------------------------------------------------
#ifdef __cpp_lib_hardware_interference_size
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Winterference-size"
  constexpr std::size_t kDestructive  = std::hardware_destructive_interference_size;  // keep APART
  constexpr std::size_t kConstructive = std::hardware_constructive_interference_size; // keep TOGETHER
  #pragma GCC diagnostic pop
#else
  constexpr std::size_t kDestructive  = 64;
  constexpr std::size_t kConstructive = 64;
#endif

// ---------------------------------------------------------------------------
// 1. alignas on a MEMBER raises the alignment of the whole class
// ---------------------------------------------------------------------------
struct MyClass {
    alignas(16) int q;  // offset 0, forced to a 16-byte boundary
    double k;           // offset 8 (next 8-aligned slot), size 4 -> padded
};
static_assert(alignof(MyClass) == 16);  // class takes the max over its members
static_assert(sizeof(MyClass)  == 16);  // size is rounded up to a multiple of alignment

// ---------------------------------------------------------------------------
// 2. alignas on the CLASS: alignment AND size round up to 32
// ---------------------------------------------------------------------------
struct alignas(32) MyClass2 {
    int q;
    double k;
};
static_assert(alignof(MyClass2) == 32);
static_assert(sizeof(MyClass2)  == 32);  // 16 bytes of padding added

// ---------------------------------------------------------------------------
// 3. False sharing: two atomics in one cache line vs. one per line
// ---------------------------------------------------------------------------
struct SharedLine {                          // both counters in the SAME line
    std::atomic<std::uint64_t> a{0};
    std::atomic<std::uint64_t> b{0};
};

struct SeparateLines {                       // each counter owns its line
    alignas(kDestructive) std::atomic<std::uint64_t> a{0};
    alignas(kDestructive) std::atomic<std::uint64_t> b{0};
};
static_assert(alignof(SeparateLines) == kDestructive);
static_assert(sizeof(SeparateLines)  == 2 * kDestructive);  // padding after each member

// ---------------------------------------------------------------------------
// 4. Constructive interference: data used together should fit in ONE line
// ---------------------------------------------------------------------------
struct alignas(kConstructive) HotData {      // e.g. fields read on every request
    std::uint32_t id;
    std::uint32_t flags;
    double        price;
};
static_assert(sizeof(HotData) <= kConstructive, "HotData should fit in one cache line");

template <class Counters>
double bench(const char* name) {
    Counters c;
    constexpr std::uint64_t N = 50'000'000;
    auto t0 = std::chrono::steady_clock::now();
    std::thread t1([&] { for (std::uint64_t i = 0; i < N; ++i) c.a.fetch_add(1, std::memory_order_relaxed); });
    std::thread t2([&] { for (std::uint64_t i = 0; i < N; ++i) c.b.fetch_add(1, std::memory_order_relaxed); });
    t1.join(); t2.join();
    double ms = std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - t0).count();
    std::cout << name << ": " << ms << " ms\n";
    return ms;
}

int main() {
    std::cout << "destructive=" << kDestructive << " constructive=" << kConstructive << "\n\n";

    // --- types ---
    std::cout << "alignof(MyClass)=" << alignof(MyClass) << " sizeof=" << sizeof(MyClass) << '\n';
    std::cout << "alignof(MyClass2)=" << alignof(MyClass2) << " sizeof=" << sizeof(MyClass2) << '\n';

    // alignof takes a TYPE in standard C++; alignof(x) on an expression is a GCC/Clang extension.
    int x = 0;
    std::cout << "alignof(decltype(x))=" << alignof(decltype(x)) << '\n';

    // --- stack objects: alignas on a variable ---
    alignas(64) char buf[64];                // buffer starting on a cache-line boundary
    alignas(MyClass2) unsigned char raw[sizeof(MyClass2)];  // align like another type
    std::cout << "buf % 64   = " << reinterpret_cast<std::uintptr_t>(buf) % 64 << '\n';
    std::cout << "raw % 32   = " << reinterpret_cast<std::uintptr_t>(raw) % 32 << '\n';
    (void)x;

    // --- heap: C++17 'new' honours over-alignment automatically ---
    auto heap = std::make_unique<SeparateLines>();
    std::cout << "heap % " << kDestructive << " = "
              << reinterpret_cast<std::uintptr_t>(heap.get()) % kDestructive << "\n\n";

    // --- offsets inside SeparateLines ---
    std::cout << "offset a=" << offsetof(SeparateLines, a)
              << " b=" << offsetof(SeparateLines, b) << "\n\n";

    // --- false sharing demo ---
    double slow = bench<SharedLine>   ("same cache line   ");
    double fast = bench<SeparateLines>("separate lines    ");
    std::cout << "speedup ~" << slow / fast << "x\n";
}
