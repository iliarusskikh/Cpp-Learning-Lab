#include <iostream>
#include <vector>
#include <functional>  // for std::function (optional but recommended)

using namespace std;

/*

// Normal pointer to a function
int (*func_ptr1)(int, int);

// Using typedef with pointer
// to a function
typedef int (*func_ptr2)(int, int);

// Function to multiply two numbers
int product(int u, int v) { return u * v; }




// Define a callback function type
typedef void (*CallbackFunction)();
typedef int (*myfunc)(int);

// Function that takes a callback function as an argument
void performAction(CallbackFunction callback) {
    std::cout << "Performing some action...\n";
    // Call the callback function
    callback();
}

// Example callback function
void myCallback() {
    std::cout << "Callback function \n";
}

// Function declarations
int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}


int fun(int i)
{
    return 0;
}

typedef void(*funpoint)(int);
typedef int(*funarray[4])(int);



typedef void(*function)(); //define the type

void HelloWorld(){
    std::cout <<"Hello"<< std::endl;
}

void PrintValue(int value){
    std::cout << value <<std::endl;
}

void ForEach(const std::vector<int>&values, void(*func)(int)){
    for(int value : values){
        func(value);
    }
}

int main()
{
    function jello = HelloWorld; //creating a variable of that type and assign to HelloWorld
    jello();
    //either
    //auto function = HelloWorld;
    //function();
    
    std:::vector<int> values = {1,4,6,3,5};
    ForEach(values, PrintValue); //pass function as parameter
 //    //you pass lambdas to function parameter which expects function ptr

    ForEach(values, [](int value) {std::cout<<value<<std::endl}); //lambda
    
    
    
    
    myfunc lambdafun;
    lambdafun=fun;
    lambdafun(1);
    // Pass the callback function to performAction
    performAction(myCallback);
    
    int (*funcArray[3])(int, int) = { add, subtract, multiply };
    std::cout << "Add: " << funcArray[0](1, 2) << std::endl;       // Calls add(10, 5)
        
    
    
    
    
    func_ptr1 = &product;

    // Using typedefed function
    // pointer for creating new
    // function pointer "new_func"
    func_ptr2 new_func_ptr = &product;

    // Using normal pointer to a function
    int x2 = (*func_ptr1)(3, 2);

    // Using the new function pointer
    int x1 = (*new_func_ptr)(2, 4);

    cout << x1 << endl;
    cout << x2;
    
    
    
    return 0;
}

*/

/*



 // Function that takes a function pointer as parameter
 void performOperation(int x, int y, int (*operation)(int, int)) {
     int result = operation(x, y);
     std::cout << "Result = " << result << "\n";
 }

 int main()
 {
     int a = 10, b = 4;

     // Using function pointers (old-school style)
     std::cout << "Using function pointers:\n";

     int (*funcPtr)(int, int) = nullptr;

     funcPtr = add;
     performOperation(a, b, funcPtr);           // Result = 14

     funcPtr = subtract;
     performOperation(a, b, funcPtr);           // Result = 6

     // You can also write it directly (more common style)
     performOperation(a, b, multiply);          // Result = 40

     // Array of function pointers (very typical use-case)
     int (*operations[])(int, int) = {add, subtract, multiply};

     std::cout << "\nUsing array of function pointers:\n";
     for (auto op : operations) {
         performOperation(a, b, op);
     }

     return 0;
 }
 */


// Some ordinary functions with the same signature
int add(int a, int b) {
    return a + b;
}

int subtract(int a, int b) {
    return a - b;
}

int multiply(int a, int b) {
    return a * b;
}
 
 //new style

 void performOperation(int x, int y, std::function<int(int,int)> operation) {
     int result = operation(x, y);
     std::cout << "Result = " << result << "\n";
 }

 int main()
 {
     int a = 10, b = 4;

     std::cout << "\n=== Using lambdas ===\n";

     // Lambda — inline, no need to declare separate named function
     performOperation(a, b, [](int x, int y){ return x + y; });     // 14

     performOperation(a, b, [](int x, int y){ return x - y; });     // 6

     performOperation(a, b, [](int x, int y){ return x * y; });     // 40

     // Even nicer: capture variables from surrounding scope
     int factor = 3;
     performOperation(a, b, [factor](int x, int y){return (x + y) * factor;});   // (10+4)*3 = 42

     // Very compact with auto parameter (C++14+)
     performOperation(a, b, [](auto x, auto y){ return x * y * 2; }); // 80

     
     std::function<int(int,int)> fuu;
     fuu = subtract;
     fuu(5,3);
     
     return 0;
 }
 
