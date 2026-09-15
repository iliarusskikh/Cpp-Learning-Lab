#include <iostream>

class A {
  int foo = 0;

public:
  int& getFoo() { return foo; }
  void printFoo() { std::cout << foo; }
};

int main() {
  A a;

  auto bar = a.getFoo();
  ++bar;

  a.printFoo();
}
// Output: 0
/*
 So the type of the expression a.getFoo() that is used for deduction is int, not int&. Now int is being used for deducing T in our imaginary void f(T t), so T is int. Since auto uses the same rule, the type of bar is also int.

 Since bar is not a reference, incrementing it does not increment foo, and the output of the program is 0.
 */
