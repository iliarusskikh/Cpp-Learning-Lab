#include <iostream>

int main() {
    const int x = 2;
    switch (x) {
        case 1:
            std::cout << "Choice 1\n";
            break;
        case 2:
            std::cout << "Choice 2\n";
            break;
        default:
            std::cout << "other\n";
            break;
    }
    return 0;
}
