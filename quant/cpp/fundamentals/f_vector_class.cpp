#include <iostream>
#include <vector>
#include <utility>
//Vector DATA on HEAP;
//Vector OBJECT on STACK

//vector class implementation
template<typename T>
class Vector
{
public:
    Vector()
    {
        //allocate 2 elements
        ReAlloc(2);
    }
    
    ~Vector()
    {
        Clear();
        ::operator delete(m_Data, m_Capacity * sizeof(T));
        //delete[] m_Data;
    }
    
    void PushBack(const T& value){
        
        if(m_Size >=m_Capacity)
        {
            ReAlloc(m_Capacity + m_Capacity /2);
        }
        new(&m_Data[m_Size]) T(value); //Avoids allocating new memory; only calls the constructor
        //m_Data[m_Size] = value;
        m_Size++;
    }
    
    void PushBack(T&& value){//no copying
        
        if(m_Size >=m_Capacity)
        {
            ReAlloc(m_Capacity + m_Capacity /2);
        }
        new(&m_Data[m_Size]) T(std::move(value)); //keep it r-value
        m_Size++;
    }
    
    void PopBack()
    {
        if(m_Size > 0)
        {
            m_Size--;
            m_Data[m_Size].~T(); //destroys the object. when size K, arr[K-1].
        }
    }
    
    void Clear()
    {
        for(size_t i = 0; i < m_Size; i++)
            m_Data[i].~T();
        
        m_Size = 0;
    }
    

    
    template<typename... Args>
    T& EmplaceBack(Args&&... args){
        if(m_Size >=m_Capacity)
        {
            ReAlloc(m_Capacity + m_Capacity /2);
        }
        
        new(&m_Data[m_Size]) T(std::forward<Args>(args)...);
        //m_Data[m_Size] = T(std::forward<Args>(args)...);
        return m_Data[m_Size++];
    }
    
    
    const T& operator[](size_t index) const
    {//second const promises not to modify the object's data members m_SIze, m_Data..
        if (index >= m_Size)
        {
            throw std::out_of_range("Index out of bounds");
        }
        return m_Data[index];
    } // if we want to see an element in an array
    
    T& operator[](size_t index)
    {
        if (index >= m_Size)
        {
            throw std::out_of_range("Index out of bounds");
        }
        return m_Data[index];
    } // if we want to modify an element in the array
    
    size_t Size() const {return m_Size;}
    
private:
    void ReAlloc(size_t newCapacity){
        //1. allocate a new block of memory
        //2. copy(move) all existing elements into a new block of elements
        //3. delete
        
        //T* newBlock = new T[newCapacity];
        T* newBlock = (T*)::operator new(newCapacity * sizeof(T));
        //check if size is growing or reducing
        if (newCapacity < m_Size)
            m_Size = newCapacity;
        
        for (size_t i = 0; i < m_Size; i++)
        {
            newBlock[i] = std::move(m_Data[i]);
        }
        
        for (size_t i = 0; i < m_Size; i++)
        {
            m_Data[i].~T();
        }
        
        ::operator delete(m_Data, m_Capacity*sizeof(T));
        //delete[] m_Data;
        
        m_Data = newBlock;
        m_Capacity = newCapacity;
        
        
    }
private:
    T* m_Data = nullptr;
    size_t m_Size = 0; //number of elements inside vector
    size_t m_Capacity =0; //how much memory is allocated
    //we will grow by half of current capacity
    
};


struct Vector3
{
    float x = 0.0f,y=0.0f,z=0.0f;
    Vector3() {}
    Vector3(float scalar) : x(scalar), y(scalar), z(scalar) {}
    Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3(const Vector3& other) //copy constructor
        : x(other.x), y(other.y), z(other.z)
    {
        std::cout << "Copy\n";
    }
    
    Vector3(Vector3&& other) //move constructor
    : x(other.x), y(other.y), z(other.z)
    {
        std::cout << "Move\n";
    }
    
    ~Vector3()
    {
        std::cout << "Destroy\n";
    }
    
    Vector3& operator=(const Vector3& other)
    {
        std::cout << "Copy\n";
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
    
    Vector3& operator=(Vector3&& other)
    {
        std::cout << "Move\n";
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
    
    
};





static int s_AllocationCount=0;

void* operator new(size_t size){
    std::cout << "Allocated" << size << " bytes\n";
    s_AllocationCount++;
    return malloc(size);
}
//overwrites NEW operator used to allocate memory


template<typename T>
void PrintVector(const Vector<T>& vector)
{
    for (size_t i =0; i < vector.Size(); i++){
        std::cout << vector[i] << std::endl;
    }
}

template<>
void PrintVector(const Vector<Vector3>& vector)
{
    for (size_t i =0; i < vector.Size(); i++){
        std::cout << vector[i].x << ", " << vector[i].y << ", "<< vector[i].z  << std::endl;
    }
}



int main(){
    //vector<int> vec (10,2);
    //sizeof(std::string) = 32 (GCC/Clang, 64-bit)
    //std::cout << sizeof(std::string);
    Vector<Vector3> vectorr;
    //vectorr.PushBack(1);
    vectorr.PushBack(Vector3(1.0f));
    vectorr.EmplaceBack(Vector3(2,3,4));
    //total allocations 64. Since Round up to a multiple of 16 bytes (or system-specific alignment).
    
    //vectorr.PushBack("You");
    //vectorr.PushBack("Hi");

    PrintVector(vectorr);
    
    Vector<int> myvec;
    myvec.EmplaceBack(3);
    

    
    return 0;
    
}
