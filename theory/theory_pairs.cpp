// CPP program to illustrate Pair in STL
#include <iostream>
#include <utility> // for std::pair and std::make_pair
#include <tuple>
#include <priority_queue>
#include <functional> //for std::ref


typedef std::pair<int, std::pair<int, char>> Pair;
//or
using PairExample = std::pair<int, std::pair<int, char>>;

//or even better
template<typename T>
using MyPair = std::pair<int, T>;).

std::pair<int, double> processValues(int inputInt, double inputDouble) {
    // Example processing
    int resultInt = inputInt * 2;
    double resultDouble = inputDouble + 1.0;
    
    return std::make_pair(resultInt, resultDouble);  // Return using make_pair
}

// Driver Code
int main()
{
    // defining a pair
    std::pair<int, char> PAIR1;
    std::pair<string, double> PAIR2("Hello", 1.23);
    std::pair<int, char> PAIR3;
    PAIR3 = std::make_pair(4,'b');
    int z = 3;
    std::pair<int, char> PAIR4 = std::make_pair(4,std::ref(z));
    z = 5; //referenced value changed.

    PAIR3 = {4,'v'};
    
    // first part of the pair
    PAIR1.first = 100;
    // second part of the pair
    PAIR1.second = 'G';

    std::cout << PAIR1.first << " ";
    std::cout << PAIR1.second << std::endl;
    
    
    std::pair<int, std::pair<int, int> > pair3 = { 3, { 4, 7 } };

    int a;
    int b = 0;
    std::pair<int, int> pair2 = { 3, 4 };
    std::tie(a, ignore) = pair2; //unpacking pairs
    std::tie(a, b) = pair3.second;
    // prints old value of b
    std::cout << a << " " << b << "\n";

    
    
    // Priority queue to process tasks by priority (highest first)
    std::priority_queue<Pair> pq;

    // Sample tasks: {priority, {task_id, type}}
    Pair tasks[] = {{5, {1, 'A'}}, {5, {2, 'B'}}, {3, {3, 'C'}}};
    int n = 3;

    // Push tasks into priority queue
    int i = 0;
    while (i < n) {
        pq.push(tasks[i]);
        i++;
    }

    // Process tasks
    while (!pq.empty()) {
        Pair task = pq.top();
        pq.pop();
        std::cout << "Processing task ID " << task.second.first
                  << " with priority " << task.first
                  << " and type " << task.second.second << std::endl;
    }
    
    
    std::pair<int, int> pairr1 = {1, 2};
    int aa = 0;
    int bb = 0;
    std::tie(aa, bb) = pairr1;
    std::cout << a << ' ' << b << '\n';

    std::pair<int, int> pairr2 = {3, 4};
    std::tie(aa, std::ignore) = pairr2;
    std::cout << aa << ' ' << bb << '\n';

    std::pair<int, std::pair<int, char>> pairr3 = {3, {4, 'a'}};
    int x = pairr3.first;
    int y = 0;
    char z = 0;
    std::tie(y, z) = pair3.second;
    std::cout << x << ' ' << y << ' ' << z << '\n';
    
    
    auto result = processValues(21, 2.14);  // Call function, get a std::pair
    
    // Access using std::get<index>
    std::cout << "Processed integer (get<0>): " << std::get<0>(result) << std::endl;
    std::cout << "Processed double (get<1>): " << std::get<1>(result) << std::endl;

    // You could also use .first and .second (more common with pair)
    std::cout << "Same via .first: " << result.first << std::endl;
    std::cout << "Same via .second: " << result.second << std::endl;
    
    return 0;
}

