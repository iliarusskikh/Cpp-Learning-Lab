#include <iostream>

void f()
{
    std::cout << "1";
}

template<typename T>
struct B
{
    void f()
    {
        std::cout << "2";
    }
};

template<typename T>
struct D : B<T>
{
    void g()
    {
        f();
    }
};

int main()
{
    D<int> d;
    d.g();
}
// Output:1
//f in the function call f() is unqualified and non-dependent, so unqualified name lookup takes place when the template definition is examined, skipping the dependent base class B and binding the name to the global declaration of f.

