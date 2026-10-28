// LIFO - last in first out
//
#include <iostream>
#include <stack>

int main() {
    std::stack<int> stack;
    stack.push(21);// The values pushed in the stack should be of the same data which is written during declaration of stack
    stack.push(22);
    stack.push(24);
    stack.push(25);
    stack.emplace(40);
    
    
    int num=0;
    
    stack.push(num);
    stack.pop();
    stack.pop();
    stack.pop();
  
    while (!stack.empty()) {
        std::cout << stack.top() <<" ";
        std::cout << stack.size()<<"\n";
        stack.pop();
    }
    
    std::stack<int> stack2 ({-2,3,6,10,4}); // initialiser list

    return 0;
}

