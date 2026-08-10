#include <iostream>

struct A {
    A() { std::cout << "A"; }
    ~A() { std::cout << "a"; }
};

int main() {
    std::cout << "main";
    return sizeof new A;
}

// Output: main
//there's also the sizeof expression form (e.g. sizeof new A) which is used in this question.
//So the expression new A is not evaluated, and A is never constructed. The expression is only used for sizeof.


