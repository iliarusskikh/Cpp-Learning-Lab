#include <iostream>
#include <template>

template <typename T>
class Stack {
public:
    // Correct: do NOT repeat <T> here
    Stack(Stack const&);          //this is the idiomatic way
    // or even better (C++11+):
    Stack(Stack const&) = default;
    // or
    Stack(const Stack&) = default;
};

//Inside the class definition (or in a member function definition that appears inside the class),
//the name Stack already refers to Stack<T> automatically — the template parameter is in scope.

template <typename T>
class Stack2 {
public:
    // Also compiles — but very uncommon
    Stack2(Stack2<T> const&);
    // or
    Stack2(const Stack2<T>&);
};


template <typename T>
class Dynamic {
public:
    //virtual ~Dynamic(); //INVALID : Declaration only
    //template <typename U> ~Dynamic();   // illegal
    // Declaration + definition (body) at the same place
    
    virtual ~Dynamic() { } //VALID way
    
    
    template <typename U>
    virtual void copy(const U&); //it is not allowed to declare a virtual member function that is also a function template.
    //Virtual functions need fixed positions in the virtual table (vtable)
};


template <typename T>
void print(T t){
    std::cout << "1 ";
}

template <>
void print(int t){
    std::cout << "2 ";
}

void print(int t){
    std::cout << "3 ";
}
//Ordinary (non-template) functions beat function templates in normal overload resolution — even if there's a perfect specialization.
//Writing <> or <T>turns off the non-template overload — only templates compete.
//Full specializations are only visible when the primary template was already chosen.
//This is why many experts recommend:
//avoid function template specializations for this exact reason — they behave unintuitively compared to class template specializations.


template <typename T>
struct Y : T {
    void bar() {std::cout <<T::identify() << "\n";}
    static std::string_view identify() {return "Y";}
};

struct Z{
    void bar() {std::cout <<identify() << "\n";}
    static std::string_view identify() {return "Z";}
};



int main(){

    Stack<int> s1; // default ctor
    
    print(0);
    print<>(0);
    print<int>(0);
    print<double>(0.);
    //output 3 2 2 1
    
    Y<Z>{}.bar();
    //output Z
    return EXIT_SUCCESS;
}

