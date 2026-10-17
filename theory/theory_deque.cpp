// CPP Program to implement Deque in STL
// allocates all of its elements and its internal pointer map on the heap
//const time for insertion, removal and access element
//reallocated, memory in blocks

#include <deque>
#include <iostream>

using namespace std;

void showdq(deque<int> g)
{
    deque<int>::iterator it;
    for (it = g.begin(); it != g.end(); ++it)
        cout << '\t' << *it;
    cout << '\n';
}

int main()
{
    deque<int> gquiz;
    gquiz.push_back(10);
    gquiz.push_front(20);
    gquiz.push_back(30);
    gquiz.push_front(15);
    cout << "The deque gquiz is : ";
    showdq(gquiz);

    cout << "\ngquiz.size() : " << gquiz.size();
    cout << "\ngquiz.max_size() : " << gquiz.max_size();

    cout << "\ngquiz.at(2) : " << gquiz.at(2);
    cout << "\ngquiz.front() : " << gquiz.front();
    cout << "\ngquiz.back() : " << gquiz.back();

    cout << "\ngquiz.pop_front() : ";
    gquiz.pop_front();
    showdq(gquiz);

    cout << "\ngquiz.pop_back() : ";
    gquiz.pop_back();
    showdq(gquiz);
    
    std::deque<int> dq = {10,22,59};
    dq.push_back(10);
    dq.push_front(20);
    dq.push_back(30);
    dq.push_front(15);
    
    //access
    int value = dq[1];
    int a = dq.at(2);//position 2
    int first = dq.back();
    int second = dq.front();
    
    dq.insert(dq.begin()+1, 14);
    dq.insert(dq.begin()+1,2,25);//2 elements
    dq.erase(dq.begin()+3);
    
    for(auto it = dq.begin(); it!=dq.end(); ++it){
        std::cout<<*it<<" ";
    }
    for (const auto& element : dq){
        std::cout << element << " ";
    }
    
    // C++20 Ranges style
    std::ranges::for_each(dq, [](int num) {
        std::cout << num << " ";
    });
    
    //size()
    //empty()
    //clear()
    return 0;
}
