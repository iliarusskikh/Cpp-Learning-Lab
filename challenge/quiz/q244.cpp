
#include <iostream>

template <typename T>
struct A {
static_assert(false);
};

int main() {
std::cout << 1;// This compiles fine now!
// A<int> broken; // Uncommenting this will trigger the compile error
}
// Output: 1

