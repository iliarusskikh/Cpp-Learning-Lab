#include <iostream>
#include <typeinfo>      // std::type_info, typeid
#include <stdexcept>     // std::bad_any_cast
#include <utility>       // std::forward, std::move (even if move is deleted)
#include <type_traits>   // std::decay_t, std::enable_if_t, std::is_same_v

namespace my_implement {

class any {
private:
    // Stores the type of the held object
    const std::type_info* type_ptr = nullptr;

    // Pointer to the heap-allocated object (always heap in this simple version)
    void* data_ptr = nullptr;

    // Helper to destroy the held object (type-erased)
    void destroy() {
        if (data_ptr && type_ptr) {
            // In real std::any this uses a virtual-like table or function pointer
            // Here we cheat a little and only support destruction via typeid check
            // (for simplicity we assume user doesn't misuse)
            // Real implementation would store a destroyer function pointer
            if (*type_ptr == typeid(int)) {
                delete static_cast<int*>(data_ptr);
            } else if (*type_ptr == typeid(double)) {
                delete static_cast<double*>(data_ptr);
            } else if (*type_ptr == typeid(std::string)) {
                delete static_cast<std::string*>(data_ptr);
            } // ... extend for more types in real code
            data_ptr = nullptr;
            type_ptr = nullptr;
        }
    }

public:
    // Default constructor → empty any
    any() = default;

    // Explicitly delete copy & move (singleton structure)
    any(const any&) = delete;
    any& operator=(const any&) = delete;
    any(any&&) = delete;
    any& operator=(any&&) = delete;

    // Templated constructor - perfect forwarding + SFINAE to avoid self-construction
    //IOW, cant execute any b1 = a1 (self type copy constructor)
    template <typename T, typename = std::enable_if_t<!std::is_same_v<std::decay_t<T>, any>>>
    any(T&& value) : type_ptr(&typeid(std::decay_t<T>))
    {
        using U = std::decay_t<T>;
        data_ptr = new U(std::forward<T>(value));
    }

    // Destructor - clean up heap object
    ~any() {
        destroy();
    }

    // Simplified any_cast (const reference version only)
    template <typename T>
    const T& any_cast() const {
        using U = std::decay_t<T>;

        if (!type_ptr || *type_ptr != typeid(U)) {
            throw std::bad_any_cast{};
        }

        return *static_cast<const U*>(data_ptr);
    }

    // Optional helpers
    bool has_value() const noexcept {
        return type_ptr != nullptr;
    }

    void reset() noexcept {
        destroy();
    }
};

} // namespace my_implement

//std::forward
//It preserves the value category (lvalue / rvalue) of the original argument when passing it further.
//In plain words:
//
//If someone passed you an lvalue (named variable, something with identity), forward gives you an lvalue
//If someone passed you an rvalue (temporary, literal, std::move(x)), forward gives you an rvalue
//
//This allows the next function / constructor to receive exactly the same kind of reference / value category that was originally passed to you.

// ──────────────────────────────────────────────
// Simple test / demonstration
// ──────────────────────────────────────────────
int main() {
    using namespace my_implement;
    using namespace std;

    any empty;
    cout << "empty.has_value() = " << empty.has_value() << "\n";

    any a = 42;
    cout << "a = " << a.any_cast<int>() << "\n";

    any b = string("hello world");
    cout << "b = " << b.any_cast<string>() << "\n";

    try {
        cout << a.any_cast<double>() << "\n";
    } catch (const bad_any_cast&) {
        cout << "Correctly caught: tried to cast int → double\n";
    }

    any c = 3.14;
    cout << "c = " << c.any_cast<double>() << "\n";

    c.reset();
    cout << "After reset, c.has_value() = " << c.has_value() << "\n";

    return 0;
}
