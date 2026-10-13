// C++ code to demonstrate working of forward_list - single linked list
//optimised for forward traversal
//more memory efficient than std::list
//no backward traversal

// push_front(), emplace_front() and pop_front()
#include <forward_list>
#include <iostream>
#include <string>

using namespace std;


struct Person{
    std::string name;
    int age;
    
    Person(std::string n, int a): name(n), age(a){
    }
    
};





// Driver Code
int main()
{
    std::forward_list<Person> people;
    people.push_front({"Alice",30});
    
    
    
    // Initializing forward list
    forward_list<int> flist = { 10, 20, 30, 40, 50 };

    // Inserting value using push_front()
    // Inserts 60 at front
    flist.push_front(60);

    // Displaying the forward list
    cout
        << "The forward list after push_front operation : ";
    for (int& c : flist)
        cout << c << " ";
    cout << endl;

    // Inserting value using emplace_front()
    // Inserts 70 at front
    flist.emplace_front(70);

    // Displaying the forward list
    cout << "The forward list after emplace_front "
            "operation : ";
    for (int& c : flist)
        cout << c << " ";
    cout << endl;

    // Deleting first value using pop_front()
    // Pops 70
    flist.pop_front();

    // Displaying the forward list
    cout << "The forward list after pop_front operation : ";
    for (int& c : flist)
        cout << c << " ";
    cout << endl;

    
    
    
    
    
    /*-----------------------*/
    
    forward_list<int> numbers = { 10, 20, 30, 40, 50 };
    forward_list<int> numbers2 = { 10, 20, 30, 40, 50 };

    for (int num : numbers){
        std::cout << num << " ";
    }
    
    for(auto it = numbers.begin(); it != numbers.end(); ++it){
        //
    }
    //use std::distance to find the number of elements between iterators
    
    auto pos = numbers.begin();
    numbers.insert_after(pos,10);
    numbers.remove(3);//all elements with value 3
    
    //.splice_after() transfers elements from another list
    numbers.splice_after(pos, numbers2);//after position
    //next sort it
    
    
    //.merge can be used for sorted
    
    forward_list<int> numbers4 = { 10, 20, 30, 40, 50 };
    auto pos = begin(numbers4);
    int distance = std::distance(begin(numbers4),end(numbers4));
    
    std::advance(pos, distance-1);
    numbers4.insert_after(pos,5);//insert in the end
    
    return 0;
}

