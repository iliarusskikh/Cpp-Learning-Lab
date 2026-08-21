#include <flat_map>
#include <iostream>

int main() {
  std::flat_map<int, int> map;
  std::cout << std::min(map[1], map[2]);
}
// Output: undefined
//An std::flat_map is a adaptor of a two containers, one for keys and one for values. Both containers are std::vector by default
//the compiler must run both map[1] and map[2] to get references to pass into std::min. However, C++ does not guarantee which one runs first.

//Memory shifts invalidate references: If the compiler decides to run map[2] first, it gets a reference pointing to key 2's position in the vector. When it runs map[1] next, key 1 gets inserted before key 2 to keep the vector sorted. This shifts key 2's position in memory or triggers a vector resize.

//Dangling reference crash: By the time std::min actually compares the two values, the reference to map[2] is pointing to memory that is no longer valid.
