//static data members
//initialised to 0 by default

#include <iostream>

class Student {
public:
    int normalCount = 0;
    static int staticCount;
    
    Student(){
        normalCount++;
        staticCount++;
    }
    
};

int Student::staticCount = 0;

int main(){
    Student s1,s2,s3;
    
    std::cout << s1.normalCount <<std::endl;
    std::cout << Student::staticCount <<std::endl;
    
    return 0;
}
