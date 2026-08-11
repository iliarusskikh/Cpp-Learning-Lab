#include <iostream>

struct Empty { };                               // sizeof = 1     (empty class rule)

struct WithNormalFunctions {
    void f() { }
    int g(double x) const { return 42; }
    void h() noexcept;
};                                              // sizeof = 1     ← same as Empty!

struct WithOneVirtual {
    virtual void v() { }
};                                              // sizeof = 8     ← vptr added

struct DataPlusNormal {
    int x;
    void print() const { std::cout << x; }
};                                              // sizeof = 4     ← only the int

struct DataPlusVirtual {
    int x;
    virtual void v() { }
};                                              // sizeof = 16    ← int (4) + padding + vptr (8)


struct A{
    A() {std::cout <<"1"<<"\n";}
    A(const A &a){std::cout <<"2"<<"\n";}
    virtual void f() {std::cout << "3"<<"\n";}
    //improve     virtual void f() const {std::cout << "3"<<"\n";}
};

struct B{
    
};

int main() {
    A a[2];
    for (auto x: a){
//  for (auto const& x: a){
        x.f();
    }

    std::cout << sizeof(B)<<"\n"; // Output = 1
    // if sizeof(B)==0 → all &arr[i] would be the same address
    //→ impossible to have distinct objects
    
    std::cout <<sizeof(A);
    //As soon as a class has at least one virtual function (or inherits one that is not overridden in a final way), the compiler must support dynamic dispatch (run-time polymorphism).
    //To make this possible, every object of such a class gets an extra hidden member:
    //vptr  (virtual table pointer / virtual pointer)
    
//    On 64-bit platforms (x86-64, ARM64, etc.):
//    sizeof(void*) = 8 bytes
//    A has no other data members
//    therefore: sizeof(A) = sizeof(vptr) = 8
    
//without virtual keyword, it outputs as 1 byte.
    
    return EXIT_SUCCESS;
}

//Output:
//1        a[0] default constructed
//1        a[1] default constructed
//2        loop: x copy-constructed from a[0]
//3        x.f()
//2        loop: x copy-constructed from a[1]
//3        x.f()

//to improve this and remove copies, pass by reference
//Improved Output:
//1        a[0] default constructed
//1        a[1] default constructed
//3        x.f()
//3        x.f()





//What contributes to the size:
// Data members (int, double, pointers, etc.) - Yes  - Stored inside each object
// Non-virtual member functions - No - Code exists once in the program (like free functions)
// Virtual member functions - Yes - (usually + sizeof(pointer)) Hidden vptr (virtual table pointer) added to every object
// Static data members - No - Stored once, outside objects
// Static member functions - No - Just namespaced functions
// Constructors / destructors - No - (unless they cause vptr)Normal ones don't;
// virtual destructor does (because it's virtual)
