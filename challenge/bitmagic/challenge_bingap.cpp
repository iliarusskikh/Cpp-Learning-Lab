/*
 finds the longest sequence of consecutive zeros surrounded by ones (often called a "binary gap") in the binary representation of a positive integer N.
 */
#include <algorithm>
#include <iostream>
#include <vector>

int solution(int N) {
    std::vector<int> v;
    while (N > 0) {
        v.push_back(N % 2);
        N /= 2;
    }
    std::reverse(v.begin(), v.end());

    int b = 0;
    int maxb = 0;
    bool started = false;
    for (int bit : v) {
        if (bit == 1) {
            if (started) {
                maxb = std::max(maxb, b);
            }
            started = true;
            b = 0;
        } else if (started) {
            ++b;
        }
    }
    return maxb;
}

int main() {
    std::cout << solution(1041) << '\n';
    return 0;
}
