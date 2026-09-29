// C++ program to show the different operator functions on async
#include <iostream>
#include <future>      // std::async, std::future
#include <thread>      // std::this_thread::sleep_for
#include <chrono>      // std::chrono literals
#include <string>
#include <vector>
#include <numeric>     // std::accumulate

using namespace std::chrono_literals;

// ────────────────────────────────────────────────
// Example 1: Very simple async task
// ────────────────────────────────────────────────
int compute_sum(int a, int b) {
    std::this_thread::sleep_for(800ms);  // simulate work
    std::cout << "  compute_sum running on thread " << std::this_thread::get_id() << "\n";
    return a + b;
}

// ────────────────────────────────────────────────
// Example 2: Returns a bigger object (string)
// ────────────────────────────────────────────────
std::string make_greeting(const std::string& name) {
    std::this_thread::sleep_for(1200ms);
    uint32_t threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());

    return "Hello, " + name + "! Welcome from async thread "
           + std::to_string(std::this_thread::get_id().hash());
}

// ────────────────────────────────────────────────
// Example 3: Heavy computation (simulated)
// ────────────────────────────────────────────────
double heavy_calculation(size_t n) {
    double sum = 0.0;
    for (size_t i = 0; i < n; ++i) {
        sum += 1.0 / (i + 1.0);
    }
    std::this_thread::sleep_for(400ms); // pretend it's expensive
    return sum;
}


void producer(std::promise<int> promise){
    int value = 54;//simulate work
    
    promise.set_value(value);//sets the result
}

void consumer(std::future<int> future){
    int result = future.get();//waits for result
    
}





int main()
{
    std::cout << "Main thread ID: " << std::this_thread::get_id() << "\n\n";

    // ────────────────────────────────────────────────
    // 1. Launch and immediately get future (most common)
    // ────────────────────────────────────────────────
    auto fut1 = std::async(std::launch::async, compute_sum, 40, 2);

    // You can do other work here...
    std::cout << "Main is doing other things...\n";

    // Later: get the result (blocks if not ready)
    int result1 = fut1.get();
    std::cout << "compute_sum(40,2) = " << result1 << "\n\n";


    // ────────────────────────────────────────────────
    // 2. Fire-and-forget style + later collect
    // ────────────────────────────────────────────────
    std::vector<std::future<double>> futures;

    for (int i = 1; i <= 6; ++i) {
        size_t work = 10'000'000 * i;
        futures.push_back(
            std::async(std::launch::async, heavy_calculation, work)
        );
    }

    std::cout << "Launched 6 background calculations...\n";

    // Collect results in order
    for (size_t i = 0; i < futures.size(); ++i) {
        double res = futures[i].get();
        std::cout << "Task " << (i+1) << " result: " << res << "\n";
    }
    std::cout << "\n";


    // ────────────────────────────────────────────────
    // 3. Using deferred execution (runs on .get() or .wait())
    // ────────────────────────────────────────────────
    auto fut_deferred = std::async(std::launch::deferred, make_greeting, "Ilia");

    std::cout << "Deferred task created, but not started yet...\n";
    std::this_thread::sleep_for(500ms);
    std::cout << "Still not started...\n";

    // Only now it runs (on the calling thread!)
    std::string greeting = fut_deferred.get();
    std::cout << greeting << "\n\n";


    // ────────────────────────────────────────────────
    // 4. Most flexible: let the system decide (async or deferred)
    //    (this is the default policy when you don't specify)
    // ────────────────────────────────────────────────
    //auto futuree = std::async([](){return compute_value_function();})
    auto future_default = std::async(make_greeting, "Modern C++");

    std::cout << "Default policy task launched...\n";
    
    //future_default.wait();
    
    //if(future_default.valid()){
        std::string msg = future_default.get();//later when we need the value
        std::cout << msg << "\n";
    //}
    

    
    

    std::cout << "\nAll tasks completed. Main exiting.\n";
    
    
    //waits for a specified duration, returns status if result is ready
    auto status = future_default.wait_for(std::chrono::seconds(2));
    
    //waits until the result is ready, but doesnt retrieve the value
    future_default.wait();
    
    //checks if the future refers to a shared state that contains a deferred function, result or exceptions
    if(future_default.valid()){
        //safe to call get();
    }

    return 0;
}

