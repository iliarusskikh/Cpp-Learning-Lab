#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <vector>

int main() {
    std::vector<int> vecs = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::sort(vecs.begin(), vecs.end(), [](int a, int b) { return std::abs(a) < std::abs(b); });
    std::transform(vecs.begin(), vecs.end(), vecs.begin(), [](int n) { return n * n; });

    std::vector<int> a = {1, 3, 5, 7};
    std::vector<int> b = {2, 4, 6};
    std::vector<int> result;
    std::merge(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(result));

    std::vector<int> v = {1, 2, 2, 3, 3, 1, 4, 4};
    std::sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());

    for (int n : result) {
        std::cout << n << ' ';
    }
    std::cout << "\nunique: ";
    for (int n : v) {
        std::cout << n << ' ';
    }
    std::cout << '\n';
    return 0;
}
