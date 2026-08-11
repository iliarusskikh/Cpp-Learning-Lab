
#include <iostream>


int& xyz(){
    int x = 10;
    return x;
}

int foo(const char*){return 1;}
int foo(short){return 2;}

int i_global;

int main() {
    int& y = xyz();
    std::cout << y <<"\n"; //undefined befaviour. function returns a reference to a variable that was destroyed -> dangling reference
    
    
    /*--------------------------------*/
    
    int x = 10;
    int y1 = 20;
    int z;
    z = x,y1; //When this whole expression is used on the right side of an assignment (=), the assignment has higher precedence than the comma.
    //parsed as (z = x) , y1;
    std::cout <<z;
    
    z = (x, y1);   // now comma has higher precedence inside () , now its 20
    
    
    /*--------------------------------*/
    //foo(0); //Compilation error, -> ambiguous
    //A null pointer constant can be converted to a pointer type via a standard conversion —
    //and this conversion is considered equally good as a promotion in many overload sets
    //involving both pointer and arithmetic overloads.
    foo(0L);               // → calls foo(short)  (0L is long → promotes to short)
    foo(static_cast<short>(0));   // explicit → calls foo(short)
    foo(nullptr);          // → calls foo(const char*)  (nullptr is pointer null)
    foo('\0');             // → calls foo(const char*)  (char literal → const char*)
    foo((short)0);         // explicit cast
    
    /*--------------------------------*/
    int* arr = new int[10];
    arr++;
    delete[] arr;
    //it is not safe, since arr is no longer pointing to the beginning of the array
    
    /*--------------------------------*/
    int x3 = 1;
    delete &x3;
    //x is a variable allocated on the stack. it has an auto storage duration. x will be cleaned up.
    //calling delete on an onject that wasnt created with new is undefined behaviour
    
    /*--------------------------------*/
    int j_local;//vs i_global
    //all global vars have static storage duration -> always zero initialised. local scope -> junk initialised. hence i_global stores 0, j_local stores junk
    
    
    int x[5] = {0,1,2,3,4,};
    //address of &x +1 -> &x[5]
    //address of x+1 -> &x[1]
    //&x → pointer to the entire array (int(*)[5])

    return EXIT_SUCCESS;
}
