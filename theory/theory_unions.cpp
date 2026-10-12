#include <iostream>

struct Entity{
    union
    {
        float a;
        int b;
        
    };
};


struct Vector2
{
    float x,y;
    
}

struct Vector4
{
    union
    {
        struct
        {
            float x,y,z,w;
        };
        
        struct
        {
            Vector2 a,b; //occupying same memory
        }
    };
}

union Data {
    int i;
    float f;
    char c;
};

int main()
{
    Entity u;
    u.a = 2.0f;
    std::cout << u.a<< " " <<u.b << std::endl;
    //u.b is int representation of a
    
    Data data{};
    data.i = 42;
    std::cout << "int " << data.i << '\n';
    data.f = 3.14f;
    std::cout << "float " << data.f << '\n';
    data.c = 'Z';
    std::cout << "char " << data.c << '\n';
    
    return 0;
}
