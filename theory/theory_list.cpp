// C++ program to demonstrate the implementation of List - double linked list implementation
//non contiguous mem allocation
//const time insertion and removal
//no random access
//bidirectional iteration

#include <iostream>
#include <iterator>
#include <list>
#include <string>
using namespace std;

// function for printing the elements in a list
void showlist(list<int> g)
{
    list<int>::iterator it;
    for (it = g.begin(); it != g.end(); ++it)
        cout << '\t' << *it;
    cout << '\n';
    
    list<int>::iterator it2 = g.begin();
    std::advance(it2, g.size()/2); //move iterator to middle position
    //for(const auto& e: g){
    //  std::cout << e << ",";
    //}
}

// Driver Code
int main()
{
    list<int> gqlist1, gqlist2;
    
    for (int i = 0; i < 10; ++i) {
        gqlist1.push_back(i * 2);
        gqlist2.push_front(i * 3);
    }
    cout << "\nList 1 (gqlist1) is : ";
    showlist(gqlist1);

    cout << "\nList 2 (gqlist2) is : ";
    showlist(gqlist2);

    cout << "\ngqlist1.front() : " << gqlist1.front();
    cout << "\ngqlist1.back() : " << gqlist1.back();

    cout << "\ngqlist1.pop_front() : ";
    gqlist1.pop_front();
    showlist(gqlist1);

    cout << "\ngqlist2.pop_back() : ";
    gqlist2.pop_back();
    showlist(gqlist2);

    cout << "\ngqlist1.reverse() : ";
    gqlist1.reverse();
    showlist(gqlist1);

    cout << "\ngqlist2.sort(): ";
    gqlist2.sort();
    showlist(gqlist2);
    
    std::list<int> numbers = {10,20,40};
    numbers.push_front(40);
    
    auto it = ++numbers.begin();
    numbers.insert(it,15);//insert in the middle

    
    
    /*--------------*/
    
    std::list<int> myList ={10,20,30};
    myList.push_back(50);
    //push_front()
    //insert()
    //emplace()
    //pop_back()
    //pop_front()
    //erase()
    //clear() O(n)
    
    std::list<int> list2(4,10); //10 10 10 10
    std::list<int> list3 = {"apple", "orange"};
    
    int first = myList.front();
    
    //list iterators
    for(std::list<int>::iterator it = myList.begin(); it != myList.end(); ++it){
        std::cout << *it <<" ";
    }
    
    //range based
    for(const auto& value: myList){
        std::cout << value << " ";
    }
    
    //.sort()
    //.unique() - remove duplicates
    //.splice(list2) - transfers elements from one list to another
    //.reverse() - reverses the order of elements
    //.merge(list2) merges sorted lists
    
    return 0;
}

