#include <iostream>

namespace namespace1 {
int age = 25;
}

namespace namespace2 {
int age = 26;
}

int main() {
    std::cout << namespace1::age << ' ' << namespace2::age << '\n';
    return 0;
}
