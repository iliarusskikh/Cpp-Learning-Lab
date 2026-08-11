#include <iostream>

class Fahrenheit{
public:
    Fahrenheit() {std::cout <<"A \n";}
    Fahrenheit(double) {std::cout<<"B \n";}
};

class RoomTemperature{
public:
    RoomTemperature() : internal_ { 2 } {
        external_ =3;//This is assignment, not initialization
//        external_ = 3 → compiler looks for operator=(double) or converting constructor
//        Fahrenheit has nooperator= But it does have Fahrenheit(double)
    }
    
    //improve with
    //RoomTemperature() : internal_{2}, external_{3} { }
private:
    Fahrenheit internal_; //constructor exists
    Fahrenheit external_; //no constructor exists
};


class A{
public:
    A() {std::cout <<"1 ";}
    ~A() {std::cout <<"2 ";}
    
};

void x(std::unique_ptr<A> ptr){
    std::cout <<"3 ";
}






int main() {

    RoomTemperature roomTemp;
    
    
    auto p1 = std::make_unique<A>(); //1 is printed
    x(std::move(p1)); //unique_ptr cant be copied, but it can be moved.
    //3 is printed in function x
    //2 is printed when moved in unique_ptr goes out of scope in the local method
    
    std::cout <<"4 ";//printed
    
    
    
    return EXIT_SUCCESS;
}

//Output:
//B
//A
//B
