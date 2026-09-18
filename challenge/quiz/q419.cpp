#include <iostream>

int main() {
  auto output = ([]{} == []{});
  std::cout << output;
}
// Output:unspecified
/*
 void(*)(). The comparison is well-formed, and what it compares is two function pointers.
it is unspecified whether the results of the conversion point to the same function, and the output of the program is unspecified.
 */
