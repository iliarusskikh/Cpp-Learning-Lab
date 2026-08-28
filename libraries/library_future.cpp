
// C++ Program to illustrate the use of std::future
#include <chrono>
#include <future>
#include <iostream>
using namespace std;

// A simple function that returns some integer value
int returnTwo() { return 2; }

// driver code
int main()
{
    // creating a future object and a thread that executes
    // the function return two asynchronously
    future<int> f = async(launch::async, returnTwo);

    // getting and printing the result
    if (f.valid()) {
        cout << f.get() << endl;
    }
    else {
        cout << "Invalid State, Please create another Task"
             << endl;
    }
    // trying for second time
    if (f.valid()) {
        cout << f.get() << endl;
    }
    else {
        cout << "Invalid State, Please create another Task"
             << endl;
    }

    return 0;
}


/*
 // C++ Program to illustrate the use of std::future
 #include <chrono>
 #include <future>
 #include <iostream>
 using namespace std;

 // A simple function that returns some integer value
 void foo(promise<int> p) { p.set_value(25); }

 // driver code
 int main()
 {
     // creating a future object and a thread that executes
     // the function return two asynchronously

     promise<int> p;
     future<int> f = p.get_future();
     ;

     // moving the task
     thread t(foo, move(p));

     t.join();
     cout << f.get();

     return 0;
 }
 
 */
