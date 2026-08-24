// https://en.cppreference.com/w/cpp/thread/barrier

#include <barrier>

#include <iostream>
#include <string>
#include <thread>  // std::jthread
#include <vector>
#include <cstdlib>

#include <syncstream>


int main()
{
    const auto workers = {"Anil", "Busara", "Carl"};

    auto on_completion = []() noexcept
    {
        // locking not needed here
        static auto phase =
            "... done\n"
            "Cleaning up...\n";
        std::cout << phase;
        phase = "... done\n";
    };

    std::barrier sync_point(std::ssize(workers), on_completion);

    auto work = [&](std::string name)
    {
        std::string product = "  " + name + " worked\n";
        std::osyncstream(std::cout) << product;  // ok, op<< call is atomic
        sync_point.arrive_and_wait();

        product = "  " + name + " cleaned\n";
        std::osyncstream(std::cout) << product;
        sync_point.arrive_and_wait();
    };

    std::cout << "Starting...\n";
    std::vector<std::jthread> threads;
    threads.reserve(std::size(workers));
    for (auto const& worker : workers)
        threads.emplace_back(work, worker);

    return EXIT_SUCCESS;
}


/*
 #include <iostream>
 #include <barrier>
 #include <thread>
 #include <vector>
 #include <syncstream> // For thread-safe std::osyncstream printing

 // Completion function executed once by one thread when all arrive
 auto on_phase_complete = []() noexcept {
     std::cout << "\n--- All threads reached barrier. Phase transition complete! ---\n\n";
 };

 // Create a barrier for 3 threads with an optional completion callback
 std::barrier sync_point(3, on_phase_complete);

 void worker(int id) {
     // Phase 1
     std::osyncstream(std::cout) << "Thread " << id << " working on Phase 1...\n";
     
     // Sync point: Decrement counter and block until all 3 threads call this
     sync_point.arrive_and_wait();

     // Phase 2 (Runs only after ALL threads finish Phase 1)
     std::osyncstream(std::cout) << "Thread " << id << " working on Phase 2...\n";
     
     // Sync point reused automatically for Phase 2
     sync_point.arrive_and_wait();
 }

 int main() {
     std::vector<std::thread> threads;
     for (int i = 0; i < 3; ++i) {
         threads.emplace_back(worker, i);
     }

     for (auto& t : threads) {
         t.join();
     }
 }
 
 */
