// FIFO

#include <iostream>
#include <queue>

//implemented using std::deque<T>, but also can use list or vector


int main()
{
    std::queue<int> q;

    // ─── Construction & assignment ───
    std::queue<int> q6({10, 20, 30});  
    std::queue<int> q2 = {10, 20, 30};         // C++11 initializer list
    q = q2;                                     // copy assignment

    // ─── Core operations ───
    q.push(100);                                // add to back
    q.emplace(200);                             // construct in place (more efficient)

    if (!q.empty()) {
        std::cout << "front = " << q.front() << "\n";   // read front
        std::cout << "back  = " << q.back()  << "\n";    // read back (C++11+)
    }

    std::cout << "size = " << q.size() << "\n"; //read size

    q.pop();                                    // remove front

    // ─── Comparison ───
    std::queue<int> q3 = {100, 200};
    if (q == q3) std::cout << "Queues are equal\n";
    if (q != q2) std::cout << "Queues are different\n";

    // swap (very efficient)
    q.swap(q3);

    std::cout << "After swap, q front = " << q.front() << "\n";

    
    
    
    //queue<CustoClass> does not need special implementation
    
    
    
    return 0;
}
