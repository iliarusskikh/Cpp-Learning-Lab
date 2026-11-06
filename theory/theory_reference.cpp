#include<iostream>
using namespace std;


void fun(int* ptr) { *ptr = 30; } // pass by ptr

void bump(int& value) { ++value; } //pass by reference



class Test
{
private:
    static int count;
public:
    Test& fun();
};

int Test::count = 0;

Test& Test::fun()
{
    Test::count++;
    cout<<Test::count<<" ";
    return *this; //returns by reference without creating a copy
    //cannot use this* for static func
}

int main()
{
    Test t;
    t.fun().fun().fun().fun();//possible because of reference
    
    //given vector fruits!
    //assume we have std::string& firstFruit = &fruits[0]; //if later we gonna push_back into that vector, it may need to resize. meaning that the memory pointer address would change. hence you would not be able to print the reference to obj
    //instead just remmeber the index, for example uint32_t firstFruit =0; then fruits[FirstFruit]
    
    int number = 66;
    int& ref = number;
    
    
    int x = 20;
    fun(&x);
    std::cout << "x = " << x << '\n';
    bump(x);
    std::cout << "x = " << x << '\n';
    
    
    return 0;
}


int& retRef(){
    int local = 100;
    int& ref = 44; //reference to temp! ISSUE!
    
    return local; //local will be destroyed!  ISSUE!
}

int& getLarger(int& a, int& b){
    return (a>b) ? a:b;
}




//The & in Test& fun() means fun() returns a reference to a Test object (not a copy or a pointer). A reference is an alias for an existing object, allowing further operations on the same object without creating a copy.
//In this case, fun() returns *this, which is a reference to the current object (t in main()). This enables method chaining, where each call to fun() returns the same object, allowing the next fun() call to be made on it.
