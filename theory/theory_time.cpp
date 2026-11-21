#include <chrono>
#include <ctime>
#include <iostream>
#include <thread>

int main() {
    using namespace std::chrono_literals;
    const auto start = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(20ms);
    const auto end = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration<float>(end - start).count() << "s\n";

    const std::time_t now = std::time(nullptr);
    std::cout << std::ctime(&now);
    return 0;
}
