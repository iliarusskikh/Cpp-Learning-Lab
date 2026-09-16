#include <iostream>
#include <utility>

class C
{
public:
    C(){}
    C(const C&){} // User-declared, disables move constructor
};

int main()
{
    C c;
    C c2(std::move(c));
    std::cout << "ok";
}

//Output:ok
/*
 So the const C& in the copy constructor is bound to the glvalue std::move(c). The copy constructor is indeed a viable function, and is used to construct c2.


 */

