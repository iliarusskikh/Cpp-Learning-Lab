#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <array>

//struct is a user-defined data type
typedef struct GFG {
    int G1;
    char G2;
    float G3;
}Kek;

struct node {
    int data1;
    char data2;
    struct node* link;
};

int main()
{
    Kek hello;

    hello.G1 = 88;
    std::cout <<hello.G1 <<"\n";

    //struct pointer
    Kek* ptrStr = &hello;
    std::cout<<ptrStr->G1<<"\n'";
    
    
    //nodes
    struct node Node1;
    Node1.link = NULL;
    Node1.data1 = 1;
    Node1.data2 = 2;
    
    struct node Node2;
    Node2.link = NULL;
    Node2.data1 = 5;
    Node2.data2 = 6;
    
    Node1.link = &Node2;
    std::cout<< Node1.link->data1 << "\n";//accessing data from Node2
    
    
    
    return 0;
}

// this is C code
/*
 
 
 #include <stdio.h>
 #include <stdlib.h>

 struct S {
     int data_length;
     int data[]; // Flexible array member (must be the last element)
 };

 int myfunction(const struct S *s) {
     int result = 0;

     for (int i = 0; i < s->data_length; i++) {
         result += s->data[i];
     }

     return result;
 }

 int main() {
     int size = 5; // Set your desired size here

     // 1. Allocate memory for struct + space for 'size' integers
 
     struct S *s = malloc(sizeof(struct S) + size * sizeof(int)); //malloc returns void* which is being casted to other types
     if (s == NULL) {
         return 1; // Handle allocation failure
     }

     // 2. Assign the size variable
     s->data_length = size;

     // 3. Populate array elements
     for (int i = 0; i < s->data_length; i++) {
         s->data[i] = (i + 1) * 10; // Assign values: 10, 20, 30, 40, 50
     }

     // 4. Call the function
     int sum = myfunction(s);
     printf("Sum: %d\n", sum); // Outputs: Sum: 150

     // 5. Clean up memory
     free(s);
     return 0;
 }
 */






//this is C++ code
/*
 
 #include <iostream>
 #include <cstdlib>

 struct S {
     int data_length;
     int data[]; // Flexible array member
 };

 // Accepts a reference to struct S
 int myfunction(const S &s) {
     int result = 0;

     for (int i = 0; i < s.data_length; i++) {
         result += s.data[i];
     }

     return result;
 }

 int main() {
     int size = 5;

     // Allocate raw memory for the struct + array elements
     S *s = reinterpret_cast<S*>(malloc(sizeof(S) + size * sizeof(int))); //or static_cast<S*>, since malloc returns void* and requires casting
     if (s == nullptr) {
         return 1;
     }

     s->data_length = size;

     for (int i = 0; i < s->data_length; i++) {
         s->data[i] = (i + 1) * 10;
     }

     // Dereference pointer 's' to pass it by reference to myfunction
     int sum = myfunction(*s);
     std::cout << "Sum: " << sum << std::endl; // Outputs: Sum: 150

     free(s);
     return 0;
 }
 */


// return by copy is safe. however, pointers would cause issues
/*
 
 //DANGEROUS: Returns a reference to a destroyed local variable
 int& func_bad() {
     int result = 43;
     return result; // Undefined Behavior!
 }

 //DANGEROUS: Returns a pointer to memory that no longer belongs to you
 int* func_bad_ptr() {
     int result = 43;
     return &result; // Undefined Behavior!
 }
 */
