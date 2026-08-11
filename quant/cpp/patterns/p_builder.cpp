/*
 #Builder Pattern
#The Builder pattern is a creational design pattern that allows constructing complex objects step-by-step. It separates the construction logic from the object's representation, enabling the same process to build different types or configurations of objects (e.g., via different builders).

 #Key characteristics:
-- Focuses on building objects with many optional parameters.
-- Avoids telescoping constructors (many overloaded constructors).
-- Provides flexibility in object creation.

#Small C++ Example: Builder for a Pizza Object
-- Here, the builder allows customizing a pizza with various toppings step-by-step.
*/


#include <iostream>
#include <string>
#include <vector>

// Product: Complex object
class Pizza {
private:
    std::string dough;
    std::string sauce;
    std::vector<std::string> toppings;

public:
    void setDough(const std::string& d) { dough = d; }
    void setSauce(const std::string& s) { sauce = s; }
    void addTopping(const std::string& t) { toppings.push_back(t); }

    void describe() const {
        std::cout << "Pizza: " << dough << " dough, " << sauce << " sauce, toppings: ";
        for (const auto& t : toppings) std::cout << t << " ";
        std::cout << "\n";
    }
};

// Builder interface
class PizzaBuilder {
public:
    virtual ~PizzaBuilder() = default;
    virtual void buildDough() = 0;
    virtual void buildSauce() = 0;
    virtual void buildToppings() = 0;
    virtual Pizza getPizza() = 0;
};

// Concrete Builder: For a specific pizza type
class MargheritaPizzaBuilder : public PizzaBuilder {
private:
    Pizza pizza;

public:
    void buildDough() override { pizza.setDough("thin crust"); }
    void buildSauce() override { pizza.setSauce("tomato"); }
    void buildToppings() override {
        pizza.addTopping("cheese");
        pizza.addTopping("basil");
    }
    Pizza getPizza() override { return pizza; }
};

// Director: Controls the building process
class PizzaChef {
private:
    PizzaBuilder* builder;

public:
    void setBuilder(PizzaBuilder* b) { builder = b; }
    Pizza construct() {
        builder->buildDough();
        builder->buildSauce();
        builder->buildToppings();
        return builder->getPizza();
    }
};

int main() {
    MargheritaPizzaBuilder margheritaBuilder;
    PizzaChef chef;
    chef.setBuilder(&margheritaBuilder);
    Pizza pizza = chef.construct();
    pizza.describe();  // Client gets customized pizza
    
    return EXIT_SUCCESS;
}


//In summary, Facade is about simplifying usage, while Builder is about flexible creation. Use Facade for subsystems and Builder for configurable objects.
