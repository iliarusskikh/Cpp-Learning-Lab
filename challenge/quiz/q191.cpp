#include<iostream>

namespace A{
  extern "C" int x;
};

namespace B{
  extern "C" int x;
};

int A::x = 0;

int main(){
  std::cout << B::x;
  A::x=1;
  std::cout << B::x;
}

// Output:01
//Due to the extern "C" specifications, A::x and B::x actually refer to the same variable.
//x is first initialized to 0, then main() starts, 0 is printed, x is incremented to 1, and finally 1 is printed.

