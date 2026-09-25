#include <cmath>
#include <iostream>

int numberOfDigits(int x) {
    int n = 0;
    while (x > 0) {
        ++n;
        x /= 10;
    }
    return n;
}

bool isArmstrong(int x) {
    const int n = numberOfDigits(x);
    int temp = x;
    int sum = 0;
    while (temp > 0) {
        const int r = temp % 10;
        sum += static_cast<int>(std::pow(r, n));
        temp /= 10;
    }
    return sum == x;
}

int main() {
    for (int n : {153, 370, 371, 407, 123}) {
        std::cout << n << (isArmstrong(n) ? " yes\n" : " no\n");
    }
    return 0;
}
