#include <iostream>
#include <thread>
#include <atomic>

std::atomic<int> flag = 0;

void waiting_thread()
{
    std::cout <<"wait for a signal\n";
    flag.wait(0); //waits until flag changes
    std::cout<<"signal received! Carry on";
}
void signaling_thread()
{
    std::this_thread::sleep_for(std::chrono::seconds(2));//imitate delay
    flag.store(1);//changing flag
    flag.notify_one();//notify thread1
}



int main()
{
    std::thread t1(waiting_thread);
    std::thread t2(signaling_thread);
    
    t1.join();
    t2.join();
    
    
    std::atomic_flag flag1 = ATOMIC_FLAG_INIT; //cannot be init with bool
    std::cout << "previous flag value: " << flag1.test_and_set() << "\n";
    flag1.clear(); //resets to 0
    
    
    return 0;
}
//allows synchronising threads without busy-waiting.

