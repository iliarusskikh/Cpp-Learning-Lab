// Object construction: instantiating, copying, assigning, destroying
// Identify differences
#include <iostream>

class obj {
public:
    obj()                    { std::cout << 1; }   // default constructor
    obj(const obj&)          { std::cout << 2; }   // copy constructor
    void operator=(const obj&) { std::cout << 3; }   // copy assignment
};

int main() {
    obj a;           // → 1          (default ctor)
    obj b{};         // → 1          (default ctor — same as obj b;)
    obj c();         // → (nothing)  declares function
    obj d = a;       // → 2          copy construction (not assignment!)
    d = b;           // → 3          copy assignment
    auto e = obj();  // → 1          default ctor (then move elision / copy elision in practice)
    obj f{a};        // → 2          copy construction (brace-init → copy ctor)
    obj g(a);        // → 2          copy construction (parentheses → copy ctor)
    auto h = b;      // → 2          copy construction (then elision possible)
    obj(i);          // → 1          declares object of type obj, named i, default-initialized (→ default ctor)

    return EXIT_SUCCESS;
}
