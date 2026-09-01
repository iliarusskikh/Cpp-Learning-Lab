#include <iostream>

// Precondition: b must not be zero
// Postcondition: r names the return value, checking it equals the division result
int divide(int a, int b)
    pre (b != 0)
    post (r: r == a / b)
{
    contract_assert(a+b==10);
    return a / b;
}

int main() {
    int result = divide(10, 2); // Valid call
    std::cout << "Result: " << result << '\n';
    
    // Calling divide(10, 0) violates the precondition and triggers a contract violation
    // divide(10, 0);
    
    return 0;
}
