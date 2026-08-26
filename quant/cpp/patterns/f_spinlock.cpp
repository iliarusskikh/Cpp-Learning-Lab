// spinlock.cpp
//
// Two spinlock implementations:
//   1) Spinlock  - built on std::atomic<bool>, with the classic
//                  "test-and-test-and-set" (TTAS) optimization plus
//                  exponential backoff to reduce cache-line contention.
//   2) Spinlock2 - built on std::atomic_flag (guaranteed lock-free,
//                  smallest possible atomic type), also upgraded to
//                  TTAS where the standard allows it.
//
// Both types satisfy the BasicLockable requirement (they expose
// lock()/unlock()), so they work directly with std::lock_guard and
// std::unique_lock without any extra wrapper code.
//
// -----------------------------------------------------------------------

#include <atomic>
#include <thread>
#include <chrono>
#include <vector>
#include <iostream>

// ---------------------------------------------------------------------
// CPU_PAUSE: portable "spin-wait" hint.
// On x86 this maps to the PAUSE instruction, which tells the CPU we're
// in a busy-wait loop. That avoids a memory-order mis-speculation
// penalty on exiting the loop and reduces power/resource contention
// with the other hyperthread sharing the core. On ARM the equivalent
// is the YIELD instruction. Everything else falls back to
// std::this_thread::yield(), which is a portable "let another thread
// run" hint (heavier than a hardware pause, but always available).
// ---------------------------------------------------------------------
#if defined(_MSC_VER)
    #include <intrin.h>
    #define CPU_PAUSE() _mm_pause()
#elif defined(__i386__) || defined(__x86_64__)
    #include <immintrin.h>
    #define CPU_PAUSE() _mm_pause()
#elif defined(__aarch64__) || defined(__arm__)
    #define CPU_PAUSE() asm volatile("yield" ::: "memory")
#else
    #define CPU_PAUSE() std::this_thread::yield()
#endif

// =========================================================================
// Spinlock — std::atomic<bool> based, test-and-test-and-set with backoff
// =========================================================================
class Spinlock {
    std::atomic<bool> locked_{false};

public:
    // Locks are not copyable or movable — copying/moving a lock that
    // may be held makes no sense and silently breaks mutual exclusion.
    Spinlock() = default;
    Spinlock(const Spinlock&)            = delete;
    Spinlock& operator=(const Spinlock&) = delete;
    Spinlock(const Spinlock&&)            = delete;
    Spinlock& operator=(const Spinlock&&) = delete;
    
    void lock() noexcept {
        // Backoff state: start by spinning with cheap CPU_PAUSE hints;
        // after enough failed attempts, fall back to yielding the OS
        // thread. This keeps latency low under light contention while
        // avoiding wasting CPU cycles (and starving other threads)
        // under heavy contention.
        constexpr int kMaxPauseIters = 16;
        int pause_iters = 1;

        for (;;) {
            // Fast path: attempt to atomically flip the lock from
            // false -> true. exchange() returns the PREVIOUS value.
            //   previous == false  -> lock was free; WE now own it -> done.
            //   previous == true   -> someone else holds it; fall through to spin.
            if (!locked_.exchange(true, std::memory_order_acquire)) {
                return; // acquired
            }

            // Slow path ("test", not "test-and-set"): spin on a plain
            // load instead of repeatedly issuing exchange(). A load
            // only needs the cache line in Shared state, whereas every
            // exchange() forces it into Exclusive/Modified state on
            // this core, invalidating other cores' copies. Reading
            // first (and only attempting exchange again once the lock
            // *looks* free) drastically cuts inter-core cache traffic
            // under contention.
            while (locked_.load(std::memory_order_relaxed)) {
                for (int i = 0; i < pause_iters; ++i) {
                    CPU_PAUSE();
                }
                // Exponential backoff, capped, to avoid a thundering
                // herd of spinners all retrying in lockstep.
                if (pause_iters < kMaxPauseIters) {
                    pause_iters <<= 1;
                } else {
                    std::this_thread::yield();
                }
            }
            // Lock looked free — loop back and race to exchange() again.
        }
    }

    bool try_lock() noexcept {
        // Cheap pre-check avoids an unconditional exchange (and the
        // cache-line ping-pong it causes) when the lock is obviously
        // held. This introduces a benign TOCTOU race: another thread
        // may lock it between the load and the exchange, but that's
        // fine — the exchange() below is still the sole source of
        // truth for whether we actually acquired it.
        return !locked_.load(std::memory_order_relaxed) && !locked_.exchange(true, std::memory_order_acquire);
    }

    void unlock() noexcept {
        locked_.store(false, std::memory_order_release);
    }

    // Non-standard convenience accessor (not part of BasicLockable) —
    // handy for diagnostics/asserts, not for making locking decisions.
    bool is_locked() const noexcept {
        return locked_.load(std::memory_order_relaxed);
    }
};

// =========================================================================
// Spinlock2 — std::atomic_flag based (smallest guaranteed-lock-free type)
// =========================================================================
class Spinlock2 {
    // Pre-C++20 requires the ATOMIC_FLAG_INIT macro for constant
    // initialization; from C++20 on, std::atomic_flag is
    // zero-initialized by default and the macro is deprecated.
#if defined(__cpp_lib_atomic_flag_test) && __cpp_lib_atomic_flag_test >= 201907L
    std::atomic_flag flag_{}; // C++20: value-initialized to clear
#else
    std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
#endif

public:
    Spinlock2() = default;
    Spinlock2(const Spinlock2&)            = delete;
    Spinlock2& operator=(const Spinlock2&) = delete;
    Spinlock2(const Spinlock2&&)            = delete;
    Spinlock2& operator=(const Spinlock2&&) = delete;
    
    void lock() noexcept {
        for (;;) {
            // test_and_set() sets the flag and returns the PREVIOUS value.
            //   previous == false -> flag was clear; we just set it -> we own the lock.
            //   previous == true  -> already set by someone else -> keep spinning.
            if (!flag_.test_and_set(std::memory_order_acquire)) {
                return;
            }

#if defined(__cpp_lib_atomic_flag_test) && __cpp_lib_atomic_flag_test >= 201907L
            // C++20 added a read-only test() so we can do the same
            // test-and-test-and-set trick as Spinlock above, instead
            // of hammering test_and_set() (which always writes) in
            // the spin loop.
            while (flag_.test(std::memory_order_relaxed)) {
                CPU_PAUSE();
            }
#else
            // Pre-C++20: std::atomic_flag has no read-only test(), so
            // the best we can do portably is pause between attempts.
            CPU_PAUSE();
#endif
        }
    }

    bool try_lock() noexcept {
        return !flag_.test_and_set(std::memory_order_acquire);
    }

    void unlock() noexcept {
        flag_.clear(std::memory_order_release);
    }
};

// =========================================================================
// Demo: several threads race to increment a shared counter through each
// lock type. Without correct mutual exclusion this would lose updates;
// the final assertions prove both locks are correct.
// =========================================================================
namespace {

template <typename LockType>
void run_demo(const char* name, int num_threads, int increments_per_thread) {
    LockType lock;
    long long counter = 0;

    std::vector<std::thread> workers;
    workers.reserve(num_threads);

    for (int t = 0; t < num_threads; ++t) {
        workers.emplace_back([&] {
            for (int i = 0; i < increments_per_thread; ++i) {
                // std::lock_guard works because both lock types expose
                // lock()/unlock() and satisfy BasicLockable.
                std::lock_guard<LockType> guard(lock);
                ++counter; // critical section
            }
        });
    }

    for (auto& w : workers) {
        w.join();
    }

    const long long expected = static_cast<long long>(num_threads) * increments_per_thread;
    std::cout << name << ": counter = " << counter
              << " (expected " << expected << ") -> "
              << (counter == expected ? "OK" : "MISMATCH") << '\n';
}

} // namespace

int main() {
    const int num_threads = std::max(2u, std::thread::hardware_concurrency());
    const int increments_per_thread = 100000;

    run_demo<Spinlock>("Spinlock ", num_threads, increments_per_thread);
    run_demo<Spinlock2>("Spinlock2", num_threads, increments_per_thread);

    return 0;
}




/*
 memory_order_relaxed
 No ordering guarantees at all beyond atomicity of the operation itself (no torn reads/writes). Other loads/stores in the same thread can be freely reordered around it. Use it when you only care that the operation is atomic — not about what it implies for other memory. Example in the spinlock: reading locked_ in the spin-wait loop is relaxed, because we're just polling ("is it still true?") — we don't need any other memory synchronized with that particular read; the real synchronization happens on the exchange that follows.

 memory_order_acquire
 Used on a load (or the load-part of a read-modify-write like exchange/test_and_set). Guarantees that no reads/writes after this operation, in program order, can be reordered before it. Practically: "once I've acquired this, I get to see everything that happened before the matching release."

 memory_order_release
 Used on a store. Guarantees that no reads/writes before this operation can be reordered after it. Practically: "everything I did before this point is now visible to whoever next acquires."

 Acquire/release pairing is what makes a lock actually work as a lock. When thread A does unlock() with release, and thread B's lock() does the acquiring exchange/load with acquire and sees the value A stored, a happens-before relationship is established: everything A did inside the critical section (before its release-store) is guaranteed visible to B once B's acquire succeeds. Without this, B could enter the critical section and see stale/partial writes from A, even though locked_ itself was correctly false→true — the lock variable would be correct, but nothing else would be guaranteed synchronized.
 
 
 
 
 */
