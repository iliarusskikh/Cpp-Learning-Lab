// C++ program to implement c++17 variant

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <iostream>

template <typename T, bool IsBigType = (sizeof(T) > 16)>
class optional;

//when type is big
template<typename T>
class optional<T, true>{
public:
    optional() = default;
    optional(T other) : pointer_{new T{other}}{
        std::cout <<"Heap implementation" << "n";
    }
    optional(const optional&) = delete;
    void operator=(const optional&) = delete;
    
    optional(const optional&&) = delete;
    void operator=(const optional&&) = delete;

    ~operator() {delete pointer_;}
    
    [[nodiscard]] bool has_value() const{
        return pointer_ != nullptr;
    }
    
    const T& value() const{
        if (!has_value()){
            throw std::logic_error("No value present!");
        }
        
        return *pointer_;
    }
    
    const T& operator*() const {
        return value();
    }
    
    explicit operator bool() const{
        return has_value();
    }
    
    
    
    
private:
    T* pointer_{ nullptr};
    
};



//when type is small
template<typename T>
class optional<T, false>{
public:
    optional() = default;
    
    optional(T other) : present_{true}{
        ::new (underlying_) T{other};
        std::cout <<"Stack implementation" << "n";
    }
    optional(const optional&) = delete;
    void operator=(const optional&) = delete;
    
    optional(const optional&&) = delete;
    void operator=(const optional&&) = delete;

    ~operator() {
        if (!present_){
            return;
        }
        std::launder(reinterpret_cast<const T*>(underlying_))->~T();
    }
    
    
    
    
    [[nodiscard]] bool has_value() const{
        return present_;
    }
    
    const T& value() const{
        if (!has_value()){
            throw std::logic_error("No value present!");
        }
        
        return *std::launder(reinterpret_cast<const T*>(underlying_));
    }
    
    const T& operator*() const {
        return value();
    }
    
    explicit operator bool() const{
        return has_value();
    }
    
    
    
    
private:
    alignas(T) std::byte underlying_[sizeof(T)];
    bool present_{false};
    
};


struct BigType{
    std::array<std::byte, 17> bytes_;
}
int main(){
    optional<int> o(2);
    BigType bigtype;
    optional<BigType> b(bigtype);
    return 0;
}
