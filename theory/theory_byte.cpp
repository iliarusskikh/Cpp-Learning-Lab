#include <array>
#include <cstddef>
#include <format>
#include <iostream>
#include <span>
#include <vector>

// -----------------------------------------------------------------------------
// 1. Custom Stream Operator
// Enable direct streaming of std::byte without modifying stream state (e.g., hex).
// -----------------------------------------------------------------------------
std::ostream& operator<<(std::ostream& os, std::byte b) {
    return os << std::to_integer<int>(b);
}

// -----------------------------------------------------------------------------
// 2. Trivially Copyable Struct
// Ensure POD (Plain Old Data) semantics for safe raw memory inspection.
// -----------------------------------------------------------------------------
struct Point {
    int x{0};
    int y{0};
};


// std::as_bytes creates a non-owning std::span<const std::byte> safely
//auto byte_view = std::as_bytes(std::span{&obj, 1});
    
// Accept a non-owning byte view directly
void print_raw_bytes(std::span<const std::byte> bytes) {
    for (std::byte b : bytes) {
        std::cout << std::format("{:02x} ", std::to_integer<int>(b));
    }
    std::cout << '\n';
}

int main() {
    // -------------------------------------------------------------------------
    // EXAMPLE 1: Vector Initialization and Formatted Display
    // -------------------------------------------------------------------------
    // Prefer initializer list or std::vector fill constructor
    std::vector<std::byte> byteVec(4, std::byte{0xFF});

    std::cout << "--- Vector Byte Display ---\n";
    for (std::byte b : byteVec) { // Use value iteration since byte is scalar
        // Output using std::to_integer, static_cast, std::format, and custom << operator
        std::cout << std::format("Int: {:<3} | Cast: {:<3} | Hex: {:02x} | Custom Stream: {}\n",
                                 std::to_integer<int>(b),
                                 static_cast<int>(b),
                                 std::to_integer<int>(b),
                                 b);
    }

    // -------------------------------------------------------------------------
    // EXAMPLE 2: Object Representation (Modern C++ Safe Memory Inspection)
    // -------------------------------------------------------------------------
    std::cout << "\n--- Object Memory Inspection ---\n";
    Point p{10, 20};
    
    std::cout << "Point struct bytes: ";
    print_raw_bytes(std::as_bytes(std::span{&p, 1}));
    //print_raw_bytes(p);
    
    // Reinterpret point memory as a sequence of raw bytes
//    const auto* bytes = reinterpret_cast<const std::byte*>(&p);
//    for (size_t i = 0; i < sizeof(Point); ++i) {
//        std::cout << std::format("{:02x} ", std::to_integer<int>(bytes[i]));
//    }
    
    
    return 0;
}
