#include <iostream>


int compare(int a, int b){
    return a > b;
}

void BubbleSort(int *A, int n, int (*compare)(int,int)){
    
}


void A(){
    
    std::cout <<"Hello" <<"\n";
}

void B(void (*ptr)()){//func ptr as argument
    
    ptr();//callback func that ptr points to
}

int add(int a, int b){
    return a+b;
}


void Increment(int* a){
    *a = (*a)+1;
}

void pprint(char* C){
    int i = 0;
    while (C[i] != '\0'){
        //*C != '\0'
        std::cout << C[i]<<"\n";
        i++;
        //C++
    }
}

void func(int (*A)[2][2]){
    //passing multi dimen arrays
}


int* Add(int* a, int* b){
    
    int c = (*a)+(*b);
    //to improve this, c needs to be allocated on heap
    return &c;
}


int Ad(int a, int b){
    return a+b;
}


int main(){
    
    void* ptr;
    int a = 50;
    ptr = (void*)&a;
    //or
    ptr = static_cast<void*>(&a);
    
    double *ptr2 = (double*)&a;
    
    
    //int = 4 bytes
    //char = 1 byte
    //float = 4 bytes
    
    //ptr = 4 bytes
    int *p;
    p = &a; //or int *p = &a;
    
    int b = 60;
    p = &b;
    
    std::cout << p << "\n";//2002
    std::cout << p+1 << "\n";//2002 + 4 = 2006 address increased
    *p = 80;//b modified
    
    void *p0;
    p0 = p;
    std::cout << p0 << "\n";
    
    int x = 9;
    void* ptr = &x; // Store the address of x in a void pointer
    std::cout << "value of ptr " << (*(int*)(ptr))<< "\n"; //dereferencing
    
    //pointer to pointer
    int **q = &p;
    //*q = address of b
    //**q = value of b
    
    //pointers as function arguements, call by pointer
    Increment(&a);
    
    //arrays
    int arr[] = {2,4,77,4,2};
    std::cout << arr << "\n";
    //same as
    std::cout << &arr[0] << "\n";
    std::cout << *arr << "\n";//first element in an array
    std::cout << *(arr+4) << "\n";//last element in an array
    
    //arr++ invalid
    int *po = A;
    po++; //works
    
    
    
    //character arrays - strings
    
    char C[4];
    C[0] = 'J';
    C[1] = 'O';
    C[2] = 'H';
    C[3] = 'N';
    C[4] = '\0';
    int len = strlen(C);
    
    //or similar string literals
    char CI[] = "John";//size is 5, length is 4;
    
    char* C2;
    C2 = C1;
    std::cout << C2[1] << "\n";
    
    pprint(CI);

    
    char CA[20] = "Hello"; //stored in stack
    char *CO = "Hello";//constant at compile time
    //cannot modify this CO[5] = 9 error
    //hence
    const char *COO = "HEHE";
    
    int br[2][3];
    int *pp[3] = br;//works!
    //br+1 would be br + 3*4 - address of the next series
    //Br[i][j] = *(Br[i] + j)
    
    
    //use heap with new and delete
    int q; //on stack
    int* w  = new int;//on heap
    //int *p = (int*)malloc(sizeof(int));
    *w = 20;
    delete w;
    //free(p);
    //for array (int*)malloc(20*sizeof(int));
    //double* numbers = new double[size]{ 0 };
    //delete[] numbers;
    
    
    
    //malloc, calloc, realloc, free
    
    int* ppp = (int*)malloc(sizeof(int));//typecasting
    *ppp = 2;
    
    //calloc, takes 2 arguments,
    int *pppp = (int*)calloc(3,sizeof(int));//number of arguments, initialises with zeroes!
    
    //realloc - change the size of mem. pointer to a previous block, and adjusted size of memory
    int *ppppp = realloc(pppp, 2*5*sizeof(int));//extends the previous block or shrinks i
    int *ppppp = realloc(pppp, 0);//free equivalent
    int *ppppp = realloc(NULL, 2*5*sizeof(int));//malloc equivalent

    
    
    
    //ptr returned by function
    int a=4, b =6;
    int* c = Add(&a, &b);//it stores the address of internal pointer. this pointer is deallocated, and that address can be overwritten in the future code
    //returning pointers from function is NotGud, only if its a heap or global section
    
    
    
    //function pointers
    int (*pe)(int,int);
    //pe = &Ad;
    //int c = (*pe)(4,6);
    
    pe = Ad;
    int c = pe(4,5);
    
    
    //callbacks
    void (*p)() = A;
    B(p);
    //pr B(A);
    
    
    //memory leak? -> when heap is not freed. holding on unused memory
    //malloc -> free
    //new -> delete
    
    Object* potr = &obj; //old style
    Object* potr = std::addressof(obj);//improved
    
    
    
    
    
    //heap
    int* ppp = new int;
    *ppp = 55;
    int vall = *ppp;
    delete ppp;
    ppp = nullptr;
    
    //function ptrs
    int(*operation)(int,int);
    operation = &add;
    int result = operation(1,4);
    
    return EXIT_SUCCESS;
}



