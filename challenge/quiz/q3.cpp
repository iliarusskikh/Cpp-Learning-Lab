#include <iostream>

void f(int) { std::cout << 1; }
void f(unsigned) { std::cout << 2; }

int main() {
  f(-2.5);
}

// compile error, overload ambiguous
//All conversions are given a rank, and both "double => int" and "double => unsigned int" are of type "floating-integral conversion", which has rank "conversion".
//no conversion is better hence ill formed
