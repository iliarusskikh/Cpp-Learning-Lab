#include <iostream>

int main() {
  void * p = &p;
  std::cout << bool(p);
}// Output:1
//The value of p is unknown, but can not be a null pointer value. The cast must thus evaluate to 1 and initialize the temporary bool as true.


