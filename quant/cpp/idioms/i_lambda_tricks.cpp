#include <iostream>


int main(){
    /*--------------------------------*/
    auto lambda = [] {};
    std::cout << sizeof(lambda); //outputs 1 byte, as it creates a struct with no private data member, and requires 1 byte to hold a unique address in memory.
//    struct __lambda_unique_name__ {
//        // no data members at all
//        constexpr void operator()() const noexcept { /* body */ }
//    };
    
    
//    int x4 = 42;
//    auto lambda2 = [x4] {};           // captures x by value
//    std::cout << sizeof(lambda2);    // → usually 4 (or 8 if int is padded)
//    struct __lambda_12345 {
//        int x;                      // ← member added
//        void operator()() const { /* uses this->x */ }
//    };
    
    
    //overloading lambdas
    auto foo = [] (double a){return a;};
    auto foo = [] (int a){return a+1;};
    
    std::cout << foo(3) <<"\n"; //compilation error
    //foo is a compiler-generated object with unique name and type.
    //not allowed to redefine lambda

    
    
    
    return EXIT_SUCCESS;
}

