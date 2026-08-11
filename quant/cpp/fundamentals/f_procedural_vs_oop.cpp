#include <iostream>
#include <string>

//procedural programming
enum Vehicle {car, bike,};

std::string giveName(Vehicle type){
    switch(type){
        case car: return "car";
        case bike: return "bike";
        default: return "";
    }
}

//object-oriented
class Veh{
public:
    std::string type;
    Veh(std::string t){
        type = t;
    }
    std::string giveName(){
        return type;
    }
};

int main() {
    Vehicle type{car};
    std::cout <<"I have a " <<giveName(type) << std::endl;
    
    Veh myVehicle("car");
    std::string type2 = myVehicle.giveName();
    std::cout <<"I have a "<<type2 << std::endl;
    
    return 0;
}
