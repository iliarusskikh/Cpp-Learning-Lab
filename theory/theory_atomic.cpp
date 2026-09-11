#include <iostream>             // for std::cout
#include <atomic>               // for std::atomic<bool>, std::atomic<int>, atomic operations
#include <thread>               // for std::thread, std::this_thread::sleep_for
#include <chrono>               // for timing literals (200ms, 50ms, etc.)
#include <vector>               // for std::vector<std::thread>

using namespace std::chrono_literals;   // lets us write 200ms instead of std::chrono::milliseconds(200)

// ───────────────────────────────────────────────
// GLOBAL VARIABLES – shared between threads
// ───────────────────────────────────────────────

// This atomic bool acts as our **spinlock** (very simple mutex-like mechanism)
std::atomic<bool> locked {false};
// starts as "unlocked" (false = free)
// only one thread should ever see it as false and change it to true at a time

// This atomic integer is used for **lock-free counting**
std::atomic<int> counter {0};
// we will safely increment it from many threads without any mutex


// ───────────────────────────────────────────────
// FUNCTION 1: Protected printing using a spinlock
// ───────────────────────────────────────────────
void safe_print(int id, const std::string& msg)
{
    // ── Try to acquire the lock ────────────────────────────────
    bool was_free = false;   // our "expected" value

    // compare_exchange_weak:  "if current value == was_free, then set it to true"
    // Returns true  → we successfully changed false → true  → we own the lock
    // Returns false → someone else took it → loop again
    while (!locked.compare_exchange_weak(was_free, true,
                                         std::memory_order_acquire,     // we want to see latest writes
                                         std::memory_order_relaxed))    // optimistic, don't need full sync on failure
    {
        was_free = false;     // VERY IMPORTANT: reset expected value!
                              // compare_exchange_weak may fail spuriously even if value didn't change
        // std::this_thread::yield();   // ← optional: give CPU to other threads (helps when very contended)
    }
    // At this point → we have the lock (nobody else is inside critical section)

    // ── Critical section ───────────────────────────────────────
    // Everything between lock acquire and release is protected
    std::cout << "[" << id << "] " << msg << "\n";
    std::this_thread::sleep_for(200ms);   // simulate some work (makes race more visible)
    // ────────────────────────────────────────────────────────────

    // ── Release the lock ───────────────────────────────────────
    locked.store(false, std::memory_order_release);
}


// ───────────────────────────────────────────────
// FUNCTION 2: Lock-free increment (no mutex needed)
// ───────────────────────────────────────────────
void count_visit(int id)
{
    // Simulate different arrival times so threads interleave more realistically
    std::this_thread::sleep_for(50ms * id);

    // fetch_add(1) does: read old value → add 1 → write new value → return old value
    // All of this happens **atomically** — no other thread can interfere midway
    int old_value = counter.fetch_add(1, std::memory_order_relaxed);
    
    int my_visitor_number = old_value + 1;

    // This print is **not** protected — numbers should still be 1..10 without duplicates/gaps
    // (because increment was atomic, even though printing is not)
    std::cout << "[" << id << "]  I am visitor #" << my_visitor_number << "\n";
}


// ───────────────────────────────────────────────
// MAIN – creates threads and waits for them
// ───────────────────────────────────────────────
int main()
{
    std::cout << "=== Part 1: Spinlock protecting cout ===\n\n";

    std::vector<std::thread> threads
    // Create 8 threads that each do "enter" + "exit"
    // → total 16 threads touching the same atomic<bool> lock
    for (int i = 1; i <= 8; ++i)
    {
        threads.emplace_back(safe_print, i, "→ Entered");
        threads.emplace_back(safe_print, i, "← Exited");
    }
    // Wait for all threads to finish
    for (auto& t : threads) t.join();

    threads.clear();   // reuse the same vector

    
    std::cout << "\n=== Part 2: Lock-free atomic counter ===\n\n";
    counter = 0;       // reset counter to zero

    // Create 10 threads that each increment the counter once
    for (int i = 1; i <= 10; ++i)
    {
        threads.emplace_back(count_visit, i);
    }
    for (auto& t : threads) t.join();

    // Read final value (load() is safe even without synchronization)
    std::cout << "\nFinal visitor count = " << counter.load() << "\n";
    
    int value = 55;
    counter.store(value, std::memory_order_release);
    //std::memory_order_release
    //std::memory_order_acquire
    //std::memory_order_relaxed
    //std::memory_order_seq_cst
    
    //.load() atomic read
    //.store() atomic write
    //.exchange() swap values atomically
    //.compare_exchange_*() compare and swap
    
    
    
    
    return 0;
}
