#include <iostream>

int main()
{
    // Use const if these values do not change during execution
    const int n = 10;
    const float f = 25.5f;
    const char c = '$';

    // Best Practice: Always initialize pointers to nullptr to avoid wild pointers
    const void* ptr = nullptr;

    // Pointing to an int (using const void* because 'n' is const)
    ptr = &n;
    std::cout << *(static_cast<const int*>(ptr)) << '\n';

    // Pointing to a float
    ptr = &f;
    std::cout << *(static_cast<const float*>(ptr)) << '\n';

    // Pointing to a char
    ptr = &c;
    std::cout << *(static_cast<const char*>(ptr)) << '\n';

    return 0;
}
