
#include <iostream>

// Move-only class example - allows moving but not copying, keeping the semantics of unique ownership
class MyMove {
    MyMove() noexcept = default; // default constructor
    MyMove(const MyMove&) = delete; // copy constructor deleted: not allowed MyMove c(a);
    MyMove& operator=(const MyMove&) = delete; // copy assignment operator deleted: not allowed MyMove c; c = a;
    MyMove(MyMove&&) = default; // move constructor defaulted: not allowed MyMove c(std::move(a));
    MyMove& operator=(MyMove&&) = default; // move assignment operator defaulted: not allowed MyMove c; c = std::move(a);
    ~MyMove() = default; // destructor defaulted
};
// Standard containers like std::vector only use the move constructor during reallocation if it's marked noexcept
// Without noexcept, the container will fall back to copying, which can be inefficient or even impossible for move-only types.




class Buffer {
public:
    // Default constructor (Ensures raw pointers don't hold garbage addresses)
    Buffer() noexcept : data_(nullptr), size_(0) {}

    // Parameterized constructor (Example implementation for completeness)
    Buffer(std::size_t size) : data_(new char[size]), size_(size) {}

    // Disable copying if you only want move semantics
    Buffer(const Buffer&) = delete;
    Buffer& operator=(const Buffer&) = delete;

    // Move constructor
    Buffer(Buffer&& other) noexcept : data_(other.data_), size_(other.size_) {
        other.data_ = nullptr; // Leave source in a safe, empty state
        other.size_ = 0;
    }
    //Buffer(Buffer&& other) noexcept : data_(std::exchange(other.data_, nullptr)),  size_(std::exchange(other.size_, 0)) {}

    // Move assignment operator
    Buffer& operator=(Buffer&& other) noexcept {
        if (this != &other) {
            delete[] data_;   // Release our own resource first
            
            data_ = other.data_;
            size_ = other.size_;
            
            other.data_ = nullptr; // Clear source resources
            other.size_ = 0;
        }
        return *this;
    }

    // Destructor
    ~Buffer() {
        delete[] data_; // Safe to call on nullptr
    }

private:
    char* data_;
    std::size_t size_;
};


int main() {
    
    return 0;
}
