#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <new>

static uint32_t s_AllocCount = 0;

void* operator new(std::size_t size) {
    ++s_AllocCount;
    std::cout << "Allocating " << size << " bytes\n";
    void* p = std::malloc(size);
    if (!p) {
        throw std::bad_alloc();
    }
    return p;
}

void operator delete(void* p) noexcept { std::free(p); }
void operator delete(void* p, std::size_t) noexcept { std::free(p); }

int main() {
    auto* p = new int(7);
    delete p;
    std::cout << "allocations: " << s_AllocCount << '\n';
    return 0;
}
