//runtime polymorphism
//dynamic binding or late binding
//without virtual functions a base class function would have been called
#include <iostream>
#include <string>
using namespace std;

class A
{
public:
    virtual void fun() { cout << "A" << endl; }
    virtual ~A() {} // Added virtual destructor - mandatory to call children destructors
    
};

class B : public A
{
public:
    virtual void fun() override{ cout << "B" << endl; } //since base class is virtual, B and C virual by default
};//override added for clarity

class C : public B
{
public:
    virtual void fun() override { cout << "C" << endl; }
};//override added for clarity

int main()
{
    // A child class derived through a base pointer
    A *a = new C; //Base class pointer to derived class
    A *b = new B;
    a->A::fun(); // Explicitly call A's fun()
    b->A::fun(); // Explicitly call A's fun()
    delete a;    // Clean up
    delete b;    // Clean up
    
    
    
    A* ptr1 = new C;
    A* ptr2 = new B;
    A* ptr3 = new A;

    ptr1->fun();   // prints C
    ptr2->fun();   // prints B
    ptr3->fun();   // prints A

    delete ptr1;
    delete ptr2;
    delete ptr3;
    
    //using smart pointers + containers
    vector<unique_ptr<A>> objects;
    
    objects.push_back(make_unique<A>());
    objects.push_back(make_unique<B>());
    objects.push_back(make_unique<C>());

    for (const auto& obj : objects) {
        obj->fun();
    }

    // no manual delete needed — RAII
    
    
    return 0;
}


//abstract classes
//a pure virtual function is declared with =0
//abstract classes cannot be instantiated
//derived class must implement all pure virtual functions






// compile time polymorphism achieved through function overloading or templates
//function binding at compile time

void print(int x){}
void print(double x){}

template<typename T>
void print(T value){}



/*
 


 class Instrument {
 public:
     virtual ~Instrument() = default;
     virtual void MakeSound() { std::cout << "Instrument\n"; }
 };

 class Accord : public Instrument {
 public:
     void MakeSound() override { std::cout << "Accord\n"; }
 };

 class Piano : public Instrument {
 public:
     void MakeSound() override { std::cout << "Piano\n"; }
 };

 class Printable {
 public:
     virtual ~Printable() = default;
     virtual std::string GetClassName() = 0;
 };

 class Entity : public Printable {
 public:
     virtual std::string GetName() { return "Entity"; }
     std::string GetClassName() override { return "GetEntity"; }
 };

 class Player : public Entity {
     std::string m_Name;

 public:
     explicit Player(const std::string& name) : m_Name(name) {}
     std::string GetName() override { return m_Name; }
     std::string GetClassName() override { return "GetPlayer"; }
 };

 void Print(Printable* obj) { std::cout << obj->GetClassName() << '\n'; }

 int main() {
     Instrument* i1 = new Accord();
     Instrument* i2 = new Piano();
     i1->MakeSound();
     i2->MakeSound();
     delete i1;
     delete i2;

     Entity* e = new Entity();
     Player* p = new Player("Lolla");
     Entity* entity = p;
     std::cout << e->GetName() << ' ' << p->GetName() << ' ' << entity->GetName() << '\n';
     Print(e);
     Print(p);
     delete e;
     delete p;
     return 0;
 }

 */
