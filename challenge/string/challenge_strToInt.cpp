//String to Integer - Write your own atoi()

//Given a string s, convert it into integer format without utilizing any built-in functions. Refer the below steps to know about atoi() function.

//
//Skip any leading whitespaces.
//Check for a sign (‘+’ or ‘-‘), default to positive if no sign is present.
//Read the integer by ignoring leading zeros until a non-digit character is encountered or end of the string is reached. If no digits are present, return 0.
//If the integer is greater than 231 – 1, then return 231 – 1 and if the integer is smaller than -231, then return -231.

#include <iostream>
#include <climits>
using namespace std;

int myAtoi(char* s) {
    int sign = 1, res = 0, idx = 0;

    // Ignore leading whitespaces
    while (s[idx] == ' ') {
        idx++;
    }

    // Store the sign of number
    if (s[idx] == '-' || s[idx] == '+') {
          if(s[idx++] == '-')
          sign = -1;
    }

    // Construct the number digit by digit
    while (s[idx] >= '0' && s[idx] <= '9') {
        
        // handling overflow/underflow test case
        if (res > INT_MAX / 10 || (res == INT_MAX / 10 && s[idx] - '0' > 7)) {
            return sign == 1 ? INT_MAX : INT_MIN;
        }
      
        // Append current digit to the result
        res = 10 * res + (s[idx++] - '0');
    }
    return res * sign;
}

int main() {
    char s[] = " -0012g4";
    cout << myAtoi(s);
    return 0;
}

/*
 
 #include <iostream>
 #include <sstream>
 #include <string>

 int main() {
     const int i_val = 20;
     const float f_val = 30.5f;

     std::stringstream ss;
     ss << i_val;
     std::string from_stream;
     ss >> from_stream;

     const std::string from_to_string = std::to_string(f_val);
     const int parsed = std::stoi("345");

     std::cout << from_stream << ' ' << from_to_string << ' ' << parsed << '\n';
     return 0;
 }

 */
