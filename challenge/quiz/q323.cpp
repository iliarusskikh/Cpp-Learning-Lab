#include <iostream>
#include <stdexcept>

struct A {
    A(char c) : c_(c) {}
    ~A() { std::cout << c_; }
    char c_;
};

struct Y { ~Y() noexcept(false) { throw std::runtime_error(""); } };

A f() {
    try {
        A a('a');
        Y y;
        A b('b');
        return {'c'};
    } catch (...) {
    }
    return {'d'};
}

int main()
{
    f();
}
// Output: bcad
//the returned object of type A is constructed. Then, the local variable b is destroyed. Next, the local variable y is destroyed, causing stack unwinding, resulting in the destruction of the returned object, followed by the destruction of the local variable a.
