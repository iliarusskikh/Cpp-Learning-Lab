//we cannot share
//no copies allowed
//auto-deletes out of scope
//we can move it
#include <iostream>
#include <memory> //unique_ptr

class Example {
public:
    Example() { std::cout << "Created" << std::endl; }
    ~Example() { std::cout << "Destroyed" << std::endl; }
};

int main() {
    // 1. Create the managed object safely
    std::unique_ptr<Example> ex = std::make_unique<Example>();

    // 2. Initialize bob as empty (no allocation overhead)
    std::unique_ptr<Example> bob = nullptr;

    // 3. Move ownership safely
    bob = std::move(ex);

    return 0;
}
