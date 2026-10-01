// C++ Program to demonstrate
// Use of template
//type safety at compile time
#include <atomics>
#include <concepts>
#include <iostream>
using namespace std;


template<typename T>
concept SupportsLessThan = requires (T x) {x < x; } //concepts

template<typename T>
requires std::copyable<T> && SupportsLessThan<T>
auto mymaxx(T a, T b){
    return b < a ? a : b;
}
// atomics not copy/move


template<typename T, typename... Types>
void print(T firstArg, Types... args){
    std::cout <<firstArg << '\n';
    if constexpr(sizeof...(args)>0){
        print(args...);
    }
}


//class tempalte
template<typename T>
class Pair{
private:
    T first;
    T second;
  
public:
    Pair(T a, T b) : first(a), second(b){}
    
    T getMax(){
        return (first > second) ? first : second;
    }
};


template<typename T, typename U = int>//default
class Container{
private:
    T data;
    T id;
  
public:
    Container(T d, U i) : data(d), id(i){}
    
    T getData(){ return data;}
    U getId(){return id;}
};


// One function works for all data types. This would work
// even for user defined types if operator '>' is overloaded
template <typename T, int N>
T myMax(T x, T y)
{
    T Arr[N];
    return (x > y) ? x : y;
}


template <typename T>
T swap(T& a, T& b){
    T temp = a;
    a = b;
    b = temp;
}
 

template<typename T, typename U>
auto add(T a, U b){
    return a + b;
}


//generic template
template<typename T>
void display(T value){
    std::cout << value << std::endl;
}

//specialised tempalte for char*
template<>
void display(char* value){
    std::cout << "String:" <<value << std::endl;
}


//C++20
//concepts allow you to constrain template parameters to specific parameters
//define a concept
template<typename T>
concept Numeric = std::is_arithmetic_v;

//use the concept to constrain a tempalte
template<Numeric T>
T add(T a, T b){
    return a+b;
}

//simplified syntax
Numeric auto multiply(Numeric auto a, Numeric auto b){
    return a* b;
}





int main()
{
    // Call myMax for int
    cout << myMax<int,4>(3, 7) << endl;
    // call myMax for double
    cout << myMax<double,5>(3.0, 7.0) << endl;
    // call myMax for char
    cout << myMax<char,1>('g', 'e') << endl;

    
    
    int x = 5, y = 10;
    swap(x,y);//compiler generates swap
    
    int num = 10;
    double val = 3.14;
    auto result1 = add(num,val);
    display(99);
    display("Heheh");
    
    return 0;
}



/*
 #include <cstddef>
 #include <iostream>
 #include <string>

 template <typename T>
 void Swap(T& a, T& b) {
     T temp = a;
     a = b;
     b = temp;
 }

 template <typename T, std::size_t S>
 class Array {
 public:
     constexpr std::size_t Size() const { return S; }
     T& operator[](std::size_t i) { return m_Data[i]; }
     const T& operator[](std::size_t i) const { return m_Data[i]; }

 private:
     T m_Data[S]{};
 };

 int main() {
     int a = 3;
     int b = 7;
     Swap(a, b);
     std::cout << a << ' ' << b << '\n';

     std::string x = "hi";
     std::string y = "yo";
     Swap(x, y);
     std::cout << x << ' ' << y << '\n';

     Array<int, 5> data;
     data[0] = 42;
     static_assert(data.Size() == 5, "unexpected size");
     std::cout << data.Size() << ' ' << data[0] << '\n';
     return 0;
 }

 */
