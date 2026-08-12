#include <iostream>

using Func = int();

struct S {
    Func f;
};

int S::f() { return 1; }

int main() {
    S s;
    std::cout << s.f();
}

// Output: 1

//using Func = int() is an alias-declaration introducing the typedef-name Func for a function taking no parameters and returning an int.
//
//The use of Func f in struct S is therefore equivalent to int f().


