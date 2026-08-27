/*
 The thread_local Storage Class is the new storage class that was added in C++11. We can use the thread_local storage class specifier to define the object as thread_local. The thread_local variable can be combined with other storage specifiers like static or extern and the properties of the thread_local object changes accordingly.

 Properties of thread_local Storage Class

 Memory Location: RAM
 Lifetime: Till the end of its thread
 */

#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <future>
#include <new> // Required for std::hardware_destructive_interference_size




// A structure designed for heavy multi-threaded contention
struct PaddedCounters {
    // Aligns this atomic to a separate cache line (e.g., 64 bytes)
    alignas(std::hardware_destructive_interference_size) std::atomic<int> worker1_count{0};
    
    // Aligns this atomic to its own separate cache line, far away from worker1_count
    alignas(std::hardware_destructive_interference_size) std::atomic<int> worker2_count{0};
};

static bool s_Finished = false;
void DoWork()
{
    using namespace std::literals::chrono_literals;
    
    while(!s_Finished){
        std::cout <<"Working...\n"; //do not use std::endl since it cout.flush() causing deadlock -> use \n instead
        std::this_thread::sleep_for(1s);
    }
    
}
// Defining thread-local variable; each thread has its own value
thread_local int val = 10;

// Mutex for synchronization
std::mutex mtx;





std::mutex m;

void worker1(int& i) {
    std::lock_guard<std::mutex>lock(m);
    for (int j = 0; j < 600; ++j) {
        std::cout << "Worker 1 processing: " << i << "\n";
        ++i;
        //std::this_thread::sleep_for(1ms);
    }
}

void worker2(int& i) {
    std::lock_guard<std::mutex>lock(m);
    for (int j = 0; j < 600; ++j) {
        std::cout << "Worker 2 processing: " << i << "\n";
        ++i;
        //std::this_thread::sleep_for(1ms);
    }
}



// Example 2

std::mutex m1;
std::condition_variable cv;
bool ready = false; // The shared condition flag
int sharedData = 0; // The actual data we want to protect

void workerTask() {
    std::cout << "[Worker] Thread started. Waiting for data...\n";

    // 1. Acquire a unique_lock (required for condition variables)
    std::unique_lock<std::mutex> lock(m1);

    // 2. Wait until the manager sets 'ready' to true
    // This line releases the lock and puts the thread to sleep!
    cv.wait(lock, [] { return ready; });

    // 3. When we get past cv.wait, the lock is automatically re-acquired!
    std::cout << "[Worker] Wake up signal received! Processing data: " << sharedData << "\n";
    
} // The lock is automatically released here when 'lock' goes out of scope

void managerTask() {
    std::cout << "[Manager] Preparing data (takes 2 seconds)...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // 1. Lock the mutex to safely update the shared variables
    {
        std::lock_guard<std::mutex> lg(m1);
        sharedData = 42;
        ready = true;
        std::cout << "[Manager] Data is ready. Notifying worker.\n";
    } // The lock_guard goes out of scope and releases the lock HERE

    // 2. Wake up one thread that is waiting on this condition variable
    cv.notify_one();
}






//Example 3: The Release-Acquire Handshake Pattern
//We use std::memory_order_release to guarantee that all writes to the payload are complete before the flag is updated, and std::memory_order_acquire to ensure the reader thread doesn't look at the payload until it sees the flag flip.

// Non-atomic data payload
struct DataPayload {
    int id;
    std::string message;
};

DataPayload shared_data;
std::atomic<bool> data_ready{false};

void producer() {
    // 1. Write to the complex, non-atomic data structure first
    shared_data.id = 101;
    shared_data.message = "Hello from the producer thread!";
    
    // Simulating slight preparation delay
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    // 2. "Publish" the data using a RELEASE store.
    // This guarantees that the writes above cannot be reordered AFTER this line.
    data_ready.store(true, std::memory_order_release);
    std::cout << "[Producer] Data published cleanly.\n";
}

void consumer() {
    // 3. Busy-wait using an ACQUIRE load.
    // No reads to 'shared_data' can be reordered BEFORE this loop completes.
    while (!data_ready.load(std::memory_order_acquire)) {
        // Spin or yield briefly to prevent pinning the CPU at 100%
        std::this_thread::yield();
    }

    // 4. Safe to read now! The release-acquire handshake establishes a
    // "happens-before" relationship, making the non-atomic writes visible here.
    std::cout << "[Consumer] Data acquired safely!\n";
    std::cout << "           ID: " << shared_data.id << "\n";
    std::cout << "           Msg: " << shared_data.message << "\n";
}



// Example 4: Lock-Free Increment via compare_exchange_weak

    std::atomic<int> global_counter{0};

    void safe_increment() {
        for (int i = 0; i < 1000; ++i) {
            // 1. Read the current value using a relaxed load (fastest, no ordering needed here)
            int old_val = global_counter.load(std::memory_order_relaxed);
            int new_val;
            
            // 2. Keep trying to update until we succeed
            do {
                new_val = old_val + 1;
                
                // compare_exchange_weak checks: "Is global_counter still equal to old_val?"
                // - If YES: it overwrites global_counter with new_val and returns TRUE (loop ends).
                // - If NO: another thread beat us to it. It overwrites our local 'old_val'
                //   with the current actual value of global_counter and returns FALSE (loop retries).
            } while (!global_counter.compare_exchange_weak(old_val, new_val, std::memory_order_relaxed, std::memory_order_relaxed));
        }
    }





// Example 5 - A simulated heavy computation function
int expensive_computation() {
    std::cout << "[Worker] Computing result in background...\n";
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate work
    return 4255; // The final result
}



//Example 6 - async
int backgroundCalculation(int x) {
    std::cout << "[Async Thread] Starting calculation...\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return x * 2;
}

int main() {
  
    std::thread th4([](){
        //new function to run!
    });
    // Created 3 threads
    // Modify value in thread 1
    std::thread th1([]() {
        val += 18;
        std::lock_guard<std::mutex> lock(mtx); //locks here mutex
        std::cout << "Thread 1 value: " << val << '\n';
    });// ← mutex is automatically unlocked here (destructor)

    std::thread th2([]() {
      
        // Modify value in thread 2
        val += 7;
        std::lock_guard<std::mutex> lock(mtx);
        std::cout << "Thread 2 value: " << val << '\n';
    });

    std::thread th3([]() {
      
        // Modify value in thread 3
        val += 13;
        std::lock_guard<std::mutex> lock(mtx); //not required but good practice
        std::cout << "Thread 3 value: " << val << '\n';
    });

    // Wait for all threads to finish
    th1.join();//joining back to main thread
    th2.join(); //joining back into main thread
    th3.join();

    // Print the value of value in the main thread
    std::cout << "Main thread value: " << val << '\n';

    
    
    // Example 3
    std::thread worker(DoWork);
    //it starts execution, including main thread before it reaches join!
    std::cin.get(); //hit enter
    s_Finished = true;
    worker.join(); //wait until worker finished
    std::cout<< "Finished!" <<std::endl;
    
    
    std::cout << "Hardware concurrency: " << std::thread::hardware_concurrency() << "\n";

    int k = 0;
    std::thread t1(worker1, std::ref(k));
    std::thread t2(worker2, std::ref(k));
    
    t1.join();
    t2.join();
    
    
    // Example 4
    std::vector<std::thread> threads;
    
    // Launch 10 threads running simultaneously
    for (int i = 0; i < 10; ++i) {
        threads.emplace_back(safe_increment);
    }
    
    // Wait for all threads to finish
    for (auto& tt : threads) {
        tt.join();
    }
    
    // Expected result: 10 threads * 1000 increments = 10000
    std::cout << "Final lock-free counter value: " << global_counter.load() << "\n";
    
    
    
    
    //Example 5:
    // 1. Create a promise-future communication channel for an integer
    std::promise<int> p;
    std::future<int> fut = p.get_future();

    // 2. Launch the worker thread, passing the promise by reference (via std::ref)
    std::thread producer2([&p]() {
        try {
            // Run the heavy task
            int result = expensive_computation();
            
            // Fulfill the promise by setting the value
            p.set_value(result);
        } catch (...) {
            // If something goes wrong, you can pass exceptions across threads too:
            // p.set_exception(std::current_exception());
        }
    });

    std::cout << "[Main] Doing other work while background thread computes...\n";

    // 3. Call .get() on the future. This automatically blocks (sleeps)
    // until the producer thread calls p.set_value() above!
    int value = fut.get();
    
    std::cout << "[Main] Result successfully received: " << value << "\n";

    // 4. Clean up the thread handle
    producer2.join();

    
    
    
    //Example 6
    // Launch the task asynchronously on a new, dedicated thread
    // This skips manual thread creation, promise wiring, and explicit joining
    std::future<int> fut = std::async(std::launch::async, backgroundCalculation, 21);

    std::cout << "[Main] Doing other work while async task runs...\n";

    // .get() blocks until the async operation finishes and returns the result
    int result = fut.get();
    std::cout << "[Main] Result from async task: " << result << "\n";
    
    
    
    
    //Example 7 packaged
                // 1. Wrap the function into a packaged_task taking an int and returning an int
    std::packaged_task<int(int)> task(backgroundCalculation);

                // 2. Get the standard future from the task
    std::future<int> raw_future = task.get_future();

                // 3. Convert it to a shared_future so multiple consumer threads can call .get()
    std::shared_future<int> shared_fut = raw_future.share();

                // 4. Launch a thread to EXECUTE the packaged_task (passing the argument 10)
                // A packaged_task is callable, so we can run it like a normal function: task(10)
    std::thread execution_thread(std::move(task), 10);

                // 5. Spawn multiple worker threads that all want to observe the final result
    std::vector<std::thread> observers;
    for (int i = 1; i <= 3; ++i) {
        observers.emplace_back([i, shared_fut]() {
            std::cout << "[Observer " << i << "] Waiting for result...\n";
            
            // All threads safely call .get() on their own copy of the shared_future
            int result = shared_fut.get();
            
            std::cout << "[Observer " << i << "] Got result: " << result << "\n";
        });
    }

                // 6. Clean up everything
    execution_thread.join();
    for (auto& ttt : observers) {
        ttt.join();
    }

    
    
    
    return 0;
}

