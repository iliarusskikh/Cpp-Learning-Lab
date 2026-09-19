// memory_order_demo.cpp
//
// Demonstrates the three memory orders most commonly reasoned about by hand:
//   std::memory_order_relaxed
//   std::memory_order_release
//   std::memory_order_acquire
//
// Build:  g++ -std=c++20 -O2 -pthread memory_order_demo.cpp -o demo
// Run:    ./demo
//
// -----------------------------------------------------------------------
// Mental model
// -----------------------------------------------------------------------
// - relaxed: the operation on the atomic itself is still atomic (no torn
//   reads/writes, and all threads agree on a single modification order
//   for that one atomic variable). What relaxed does NOT give you is any
//   guarantee about the visibility/order of OTHER memory operations
//   (atomic or not) around it. Use it when you only care about the atomic
//   value itself (counters, stats) and nothing else needs to be
//   synchronized through it.
//
// - release (on a store): every ordinary memory write that happened
//   before this store, in program order, becomes visible to any thread
//   that later performs a matching acquire load and observes this
//   store's value (or a later value in the release sequence).
//release (store) is a barrier for everything before it in program order — none of those earlier operations can be reordered to happen after the release.

// - acquire (on a load): if the value read was written by a release
//   store (or a later store in its release sequence), then every write
//   that happened-before that release store is now guaranteed visible to
//   this thread from this point onward.
//acquire (load) is a barrier for everything after it in program order — none of those later operations can be reordered to happen before the acquire.
//
// Together, release + acquire on the SAME atomic form a "synchronizes-with"
// edge. That edge is what makes it safe to hand off non-atomic data (like
// shared_data below) between threads without a mutex.
// -----------------------------------------------------------------------

#include <atomic>
#include <thread>
#include <vector>
#include <iostream>
#include <cassert>
#include <string>

// =========================================================================
// Demo 1: memory_order_relaxed
//
// Several threads increment a shared counter. We don't care in what
// order the increments happen relative to any other memory, we only
// care that the final count is correct (no lost updates). relaxed is
// enough because fetch_add is still atomic - it just doesn't establish
// any happens-before relationship with other memory operations.
// =========================================================================
void demo_relaxed()
{
    std::cout << "=== Demo 1: memory_order_relaxed (atomic counter) ===\n";

    std::atomic<long> counter{0};
    constexpr int num_threads = 8;
    constexpr int increments_per_thread = 100'000;

    std::vector<std::thread> workers;
    for (int t = 0; t < num_threads; ++t) {
        workers.emplace_back([&counter]() {
            for (int i = 0; i < increments_per_thread; ++i) {
                // Only atomicity is required here - no other memory needs
                // to be ordered around this operation.
                counter.fetch_add(1, std::memory_order_relaxed);
            }
        });
    }
    for (auto& t : workers) t.join();

    long expected = static_cast<long>(num_threads) * increments_per_thread;
    std::cout << "counter = " << counter.load(std::memory_order_relaxed)
              << " (expected " << expected << ")\n\n";
    assert(counter.load() == expected);
}

// =========================================================================
// Demo 2: memory_order_release / memory_order_acquire
//
// Classic "publish a message" pattern. The producer writes ordinary
// (non-atomic) data, then publishes it by storing `ready = true` with
// release. The consumer spins on an acquire load of `ready`. Once it
// observes true, the acquire load synchronizes-with the release store,
// so the consumer is guaranteed to see the fully-written shared_data
// and message - no data race, no torn reads, despite shared_data being
// a plain (non-atomic) variable.
//
// If `ready` were relaxed on either side, this would be a data race and
// undefined behavior: the compiler/CPU would be free to reorder the
// write to shared_data past the store to ready (or the read of
// shared_data before the load of ready), and the consumer could observe
// a stale or partially-written value.
// =========================================================================
struct Message {
    int   id;
    std::string text;
};

std::atomic<bool> ready{false};
Message shared_data{};   // plain data, NOT atomic - protected by ready

void producer()
{
    shared_data.id   = 42;
    shared_data.text = "hello from producer";      // (1) ordinary writes

    ready.store(true, std::memory_order_release);  // (2) publish
}

void consumer()
{
    // (3) acquire: spin until we observe the producer's release store
    while (!ready.load(std::memory_order_acquire)) {
        std::this_thread::yield();
    }

    // Because (3) read the value written by (2), (3) synchronizes-with
    // (2). Everything sequenced-before (2) - i.e. (1) - is now guaranteed
    // visible here. This assert can never fire.
    assert(shared_data.id == 42);
    std::cout << "consumer observed: id=" << shared_data.id
              << " text=\"" << shared_data.text << "\"\n";
}

void demo_release_acquire()
{
    std::cout << "=== Demo 2: memory_order_release / memory_order_acquire ===\n";
    ready.store(false, std::memory_order_relaxed); // reset for repeat runs

    std::thread c(consumer);
    std::thread p(producer);
    p.join();
    c.join();
    std::cout << '\n';
}

// =========================================================================
// Demo 3: release/acquire building a real primitive - a minimal spinlock
//
// This is why release/acquire matter in practice: they're the building
// blocks used to implement locks. test_and_set with acquire on lock(),
// and clear with release on unlock(), gives you mutual exclusion AND the
// same "everything written while holding the lock is visible to the next
// thread that acquires it" guarantee you'd get from std::mutex.
// =========================================================================
class SpinLock {
public:
    void lock() {
        // Keep trying until we're the thread that flips the flag from
        // false -> true. Acquire on success so nothing written by the
        // previous lock-holder (before their release-unlock) can be
        // reordered ahead of this point.
        while (flag_.test_and_set(std::memory_order_acquire)) {
            std::this_thread::yield();
        }
    }

    void unlock() {
        // Release so every write made while holding the lock is visible
        // to whichever thread next successfully acquires it.
        flag_.clear(std::memory_order_release);
    }

private:
    std::atomic_flag flag_ = ATOMIC_FLAG_INIT;
};

void demo_spinlock()
{
    std::cout << "=== Demo 3: hand-rolled SpinLock using acquire/release ===\n";

    SpinLock lock;
    long protected_counter = 0;   // ordinary variable, guarded by lock
    constexpr int num_threads = 8;
    constexpr int increments_per_thread = 50'000;

    std::vector<std::thread> workers;
    for (int t = 0; t < num_threads; ++t) {
        workers.emplace_back([&]() {
            for (int i = 0; i < increments_per_thread; ++i) {
                lock.lock();
                ++protected_counter;   // safe: mutual exclusion + visibility
                lock.unlock();
            }
        });
    }
    for (auto& t : workers) t.join();

    long expected = static_cast<long>(num_threads) * increments_per_thread;
    std::cout << "protected_counter = " << protected_counter
              << " (expected " << expected << ")\n\n";
    assert(protected_counter == expected);
}

int main()
{
    demo_relaxed();
    demo_release_acquire();
    demo_spinlock();
    std::cout << "All demos completed without assertion failures.\n";
    return 0;
}
