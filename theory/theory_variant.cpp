// C++ program to show the different operator functions on std variant
//compile-time
//no dynamic allocation
#include <iostream>
#include <variant>
//type-safe union - one type at a time

//store data of multiple datatype
//if you not sure at the moment what type of data will it be


//function that can return different types
std::variant<std::string, int, double> parseInput(const std::string& input){
    if(input == "pi"){
        return 3.14159;
    }else if(input.find_first_not_of("0123456789") == std::string::npos){
        return std::stoi(input);
    } else
        return "Invalid input: "+ input;
}


int main()
{
    //c++ 17
    std::variant<std::string, int>data;//it stores all these datatypes as members
    data = "Huhuhu";
    //std::get<std::string>(data);
    std::cout << "Hello " << std::get<std::string>(data) << "\n";
    
    data.index(); //gives 0 for string and 1 for int - to understand what is the type at the moment, outputs the current index
    data= 2;
    std::cout << "Hello " << std::get<int>(data) << "\n";

    
    if (auto* value = std::get_if<std::string>(&data)){
        //means wrong type
    }else{
        std::string& v= *value;// ← no copy, just another name
        //With & you get aliasing / no copy
    }
    
    
    //visit with a lambda
    std::visit([](auto&& arg){
        std::cout << arg<<"\n";
    }, data);
    
    
    //checks if currently holds specific type
    if(std::holds_alternative<std::string>(data)){
        std::cout << get<std::string>(data);
    }
    
    return 0;
}

