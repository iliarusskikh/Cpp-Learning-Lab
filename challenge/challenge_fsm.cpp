#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

enum VendingMachineState { SELECT, QUANTITY, CALCULATE, UPDATE };
enum Brand { COKE = 100, PEPSI = 200, SEVENUP = 300 };

struct Student {
    std::string name;
    int points;
};

struct Soda {
    Brand brandName;
    int inventory;
    int price;
};

template <class T>
class State {
public:
    virtual ~State() = default;
    virtual void Enter(T* owner) = 0;
    virtual void Execute(T* owner) = 0;
    virtual void Exit(T* owner) = 0;
};

template <class T>
class StateMachine {
    T* m_pOwner = nullptr;
    State<T>* m_pCurrentState = nullptr;
    bool m_firstRun = true;

public:
    void Init(T* owner, State<T>* initialState) {
        m_pOwner = owner;
        m_pCurrentState = initialState;
    }

    void Update() {
        if (m_firstRun) {
            m_firstRun = false;
            m_pCurrentState->Enter(m_pOwner);
        }
        m_pCurrentState->Execute(m_pOwner);
    }

    void ChangeState(State<T>* newState) {
        m_pCurrentState->Exit(m_pOwner);
        m_pCurrentState = newState;
        m_pCurrentState->Enter(m_pOwner);
    }
};

class Vendor : public StateMachine<Vendor> {
public:
    bool done = false;
};

class IdleState : public State<Vendor> {
    void Enter(Vendor*) override { std::cout << "enter idle\n"; }
    void Execute(Vendor* owner) override {
        std::cout << "idle execute\n";
        owner->done = true;
    }
    void Exit(Vendor*) override { std::cout << "exit idle\n"; }
};

int sodaIndex(Brand brand) {
    switch (brand) {
        case COKE: return 0;
        case PEPSI: return 1;
        case SEVENUP: return 2;
    }
    return -1;
}

int main() {
    Soda machine[3] = {
        {COKE, 100, 1000},
        {PEPSI, 10, 2000},
        {SEVENUP, 10, 3000},
    };
    Student eric{"Eric", 100000};
    const Brand selected = COKE;
    const int quantity = 2;
    const int cost = machine[sodaIndex(selected)].price * quantity;
    const int payment = cost;

    VendingMachineState state = CALCULATE;
    if (state == CALCULATE && payment <= eric.points && payment >= cost) {
        eric.points -= cost;
        machine[sodaIndex(selected)].inventory -= quantity;
        state = UPDATE;
    }
    std::cout << eric.name << " points " << eric.points
              << " coke left " << machine[0].inventory
              << " state " << state << '\n';

    IdleState idle;
    Vendor vendor;
    vendor.Init(&vendor, &idle);
    vendor.Update();
    return 0;
}
