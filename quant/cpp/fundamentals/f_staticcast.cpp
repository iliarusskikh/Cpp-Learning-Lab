#include <iostream>
#include <string>
using namespace std;

template<typename A, typename B>
auto func1(A alpha, B beta){
return (static_cast<float>(alpha) + beta);
}

int main(){

    cout << func1(12,4.2);
    return 0;
    
    
}
