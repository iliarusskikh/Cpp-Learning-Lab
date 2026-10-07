//move semantics


//rvalue -> does not have persistent mem location, does not have mem location
//lvalue -> persistent memory location, can get an address of it
//int x = 10; //x is lvalue, 10 is rvalue

//rvalue reference binds to rvalue || int&& rr = 5;
//lvalue ref binds to lvalue || int& k = x;

//we need rvalue reference for move constructor

#include <iostream>
#include <cstring>
#include <utility>

class MyString{
    char* data;

public:
    MyString(const char* s){
        data = new char[strlen(s) + 1];//null character
        strcpy(data,s);
        std::cout << "Constructed!"<<std::endl;
    }
    
    //deep copy
    MyString(const MyString& other){//lvalue reference is defined!
        data = new char[strlen(other.data) + 1];//null character
        strcpy(data,other.data);
        //memcpy(data,other.data,strlen(other.data) + 1);
        std::cout << "Copied!\n";

    }
    
    
    
    
    // 1. Move constructor
    MyString(MyString&& other) noexcept
        : data(other.data)          // steal the pointer
    {
        other.data = nullptr;       // leave the moved-from object in valid (empty) state
        std::cout << "Move constructor called\n";
    }

    // 2. Move assignment operator
    MyString& operator=(MyString&& other) noexcept
    {
        if (this != &other)         // important: self-assignment check
        {
            // Step 1: release our current resource (if we have any)
            delete[] data;

            // Step 2: steal the resource
            data = other.data;

            // Step 3: put the moved-from object into valid empty state
            other.data = nullptr;
            
            std::cout << "Move assignment called\n";
        }

        return *this;
    }
    
    
    
    
    ~MyString(){
        delete[] data;
        std::cout << "Destroyed!\n";
    }
    
    void print(){
        if(data){//if data is nullptr
            std::cout <<data<<"\n";
        } else{
            std::cout <<"Empty\n";
        }
    }
};


int main(){
    MyString s1("Hello");
    MyString s2 = s1; //copy object of s1, s1 is lvalue
    s1.print();
    s2.print();
    
    //now we want to transfer the ownership of s1 into s2
    //hence, move constructor
    //we need to tell the compilor to read s1 as a temporary object or rvalue.
    
    MyString s3 = std::move(s1); //move constructor
    
    
    
    MyString a("hello");
    std::cout << "a: "; a.print();

    MyString b = std::move(a);          // move constructor
    std::cout << "b: "; b.print();
    std::cout << "a after move: "; a.print();   // should be null

    MyString c("world");
    c = std::move(b);                   // move assignment
    std::cout << "c: "; c.print();
    std::cout << "b after move: "; b.print();
    
    
    
    //implemented within containers std
    std::vector<std::string> v1;
    v1.push_back("hello");
    
    std::vector<std::string> v2 = std::move(v1);
    
    //types that can only be moved, not copied : unique_ptr, thread, future, promise
    
    return 0;
}
