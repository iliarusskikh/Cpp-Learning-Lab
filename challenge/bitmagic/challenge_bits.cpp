#include <iostream>

int getBit(int n, int pos) { return (n & (1 << pos)) != 0; }

int setBit(int n, int pos) { return n | (1 << pos); }

int clearBit(int n, int pos) { return n & ~(1 << pos); }

int updateBit(int n, int pos, int value) {
    n = n & ~(1 << pos);
    return n | (value << pos);
}

int modifyBit(int n, int p, int b) {
    const int mask = 1 << p;
    return (n & ~mask) | (b << p);
}

int main() {
    std::cout << getBit(5, 2) << '\n';
    std::cout << setBit(5, 1) << '\n';
    std::cout << clearBit(5, 0) << '\n';
    std::cout << updateBit(5, 1, 1) << '\n';
    std::cout << modifyBit(6, 2, 0) << '\n';
    std::cout << modifyBit(6, 5, 1) << '\n';
    return 0;
}
