// C++ to demonsrate priority queue
// max-heap by default, max element on top
// can implement min-heap
#include <iostream>
#include <queue>
#include <vector>
#include <string>

// 1. Custom struct with a member-defined comparison operator
struct Task {
    std::string name;
    int priority;

    // A max-heap puts the largest element at the top.
    // To make a higher number mean higher priority, we use '<'.
    // If you want lower numbers to have higher priority, use '>' instead.
    bool operator<(const Task& other) const {
        return priority < other.priority;
    }
};

// Helper function to print a generic priority queue (note: passes by value to preserve original)
template <typename T>
void printPriorityQueue(std::priority_queue<T> pq) {
    while (!pq.empty()) {
        std::cout << pq.top() << " ";
        pq.pop();
    }
    std::cout << '\n';
}

int main() {
    // Enable fast I/O
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::cout << "=== 1. Default Max-Heap (Integers) ===\n";
    std::priority_queue<int> maxHeap;
    maxHeap.push(3);
    maxHeap.push(1);
    maxHeap.push(33);
    maxHeap.push(32);

    std::cout << "Elements popped (highest to lowest): ";
    printPriorityQueue(maxHeap); // Will print 33 32 3 1
    std::cout << "\n";


    std::cout << "=== 2. Min-Heap using std::greater ===\n";
    // Syntax: priority_queue<T, Container, Comparator>
    std::priority_queue<int, std::vector<int>, std::greater<int>> minHeap;
    for (int val : {10, 2, 4, 8, 6, 9}) {
        minHeap.push(val);
    }

    std::cout << "Elements popped (lowest to highest): ";
    while (!minHeap.empty()) {
        std::cout << minHeap.top() << " ";
        minHeap.pop();
    }
    std::cout << "\n\n";


    std::cout << "=== 3. Custom Struct with Operator Overload (Tasks) ===\n";
    std::priority_queue<Task> taskQueue;
    taskQueue.push({"Email check", 2});
    taskQueue.push({"Coffee break", 6});
    taskQueue.push({"Server deployment", 10});

    std::cout << "Executing tasks by priority:\n";
    while (!taskQueue.empty()) {
        Task current = taskQueue.top();
        std::cout << " - Task: " << current.name << " (Priority: " << current.priority << ")\n";
        taskQueue.pop();
    }
    std::cout << "\n";


    std::cout << "=== 4. Custom Struct with Lambda Comparator (People) ===\n";
    struct Person {
        std::string name;
        int age;
    };

    // Lambda comparator: Prioritize older people (max-heap style based on age)
    auto ageComparator = [](const Person& a, const Person& b) {
        return a.age < b.age;
    };

    std::priority_queue<Person, std::vector<Person>, decltype(ageComparator)> peopleQueue(ageComparator);
    peopleQueue.push({"Alice", 25});
    peopleQueue.push({"Bob", 42});
    peopleQueue.push({"Charlie", 30});

    std::cout << "People ordered by descending age:\n";
    while (!peopleQueue.empty()) {
        Person p = peopleQueue.top();
        std::cout << " - " << p.name << " (" << p.age << " years old)\n";
        peopleQueue.pop();
    }
    std::cout << "\n";


    std::cout << "=== 5. Performance Optimization (emplace & reserve) ===\n";
    // If you know you will insert many items, you can pre-allocate vector capacity
    // to avoid multiple reallocations during heap construction.
    std::vector<int> underlyingVector;
    underlyingVector.reserve(1000); // Pre-allocate memory

    // Constructing directly from vector range
    std::priority_queue<int> optimizedPq(std::less<int>(), std::move(underlyingVector));
    
    // Use emplace() to construct elements in-place and avoid temporary copies
    optimizedPq.emplace(42);
    optimizedPq.emplace(100);

    std::cout << "Optimized queue top element: " << optimizedPq.top() << "\n";

    //example algo
    std::priority_queue<int> prq;
    prq.push(32);
    prq.push(3);
    prq.push(2);
    prq.push(36);
    while(!prq.empty()){
        int t = prq.top();
        std::cout << "number "<<t <<std::endl;
        prq.pop();
    }
    
    return 0;
}
