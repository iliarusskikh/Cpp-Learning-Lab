#include <memory>//for smart ptrs
#include <iostream>

using namespace std;

/*auto_ptr
unique_ptr
shared_ptr
weak_ptr
*/
//no need to call delete for new for ptr

int main() {
    
    // Pointer declaration
    std::auto_ptr<int> ptr1(new int(10));
    std::cout << *ptr1 << endl;
    
    // Transfer ownership to
    // pointer ptr2,
    std::auto_ptr<int> ptr2 = ptr1;
    std::cout << *ptr2;
    
    
    //unique_ptr stores one pointer only at a time. We cannot copy unique_ptr, only transfer ownership of the object to another unique_ptr using the move() method.
    std::unique_ptr<int> P1(13)
    std::unique_ptr<int> P2;

    // Copy the addres of P1 into p2
    P2 = move(P1);

    //to reset previously managed resource
    P2.reset(new int(100));
    
    
    
    //By using shared_ptr, more than one pointer can point to same object at a time, and it will maintain a reference counter using the use_count() method.
    
    std::shared_ptr<int> P1(11)
    std::shared_ptr<int> P2;
    // P1 and P2 are pointing
    // to same object
    P2 = P1;
    std::cout << P1.use_count();
    
    
    
    //weak_ptr is a smart pointer that holds a non-owning reference to an object. It's much more similar to shared_ptr except it will not maintain a reference counter.
    //breaks circular references in pointers
    // Create shared_ptr Smart Pointer
    shared_ptr<int> P1(16);
    auto ptr2 = P1;
    std::cout << P1.use_count();//outputs 2
    P1.reset();//reset one pointer
    std::cout << ptr2.use_count();//outputs 1 - decreased
    
    
    // Created a weak_ptr smart pointer
    //to be used with shared ptrs
    std::weak_ptr<int> P2(P1);
    // Returns the number of shared_ptr
    // objects that manage the object
    std::cout << P2.use_count();
    
    //if we have a shared ptr and weak ptr pointing to the same object. then if shared ptr is destroyed, weak.expired == true;
    
    
    //for classes
    std::unique_ptr<Class> C(new Class());
    std::shared_ptr<Class> C2 = std::make_shared<Class>();
    std::shared_ptr<Class> C3;
    C3 = C2;
    
    return 0;
}
