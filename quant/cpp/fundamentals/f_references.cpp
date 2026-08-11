#include <iostream>
#include <vector>



int main(){
    
    //ptrs
    int a{10};
    int* p{&a};
    std::cout << a << std::endl;
    std::cout << p << std::endl;
    std::cout << *p << std::endl;
    

    //ref
    int& ref{a};
    std::cout << a << std::endl;
    std::cout << ref << std::endl;
    
    ref = 20; //overwrites variable.
    
    return 0;
}

