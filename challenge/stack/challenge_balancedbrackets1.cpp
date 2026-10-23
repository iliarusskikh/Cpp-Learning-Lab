#include <iostream>
#include <stack>
#include <string>
#include <vector>

bool isBalanced(const std::string& s) {
    std::stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '{' || c == '[') {
            st.push(c);
        } else if (c == ')' || c == '}' || c == ']') {
            if (st.empty()) {
                return false;
            }
            const char top = st.top();
            if ((c == ')' && top != '(') || (c == '}' && top != '{') ||
                (c == ']' && top != '[')) {
                return false;
            }
            st.pop();
        }
    }
    return st.empty();
}

int main() {
    const std::vector<std::string> testCases = {"[{()}]", "[()()]{}", "(]", "([{]})"};
    for (const std::string& s : testCases) {
        std::cout << "Input: " << s << " -> " << (isBalanced(s) ? "true" : "false") << '\n';
    }
    return 0;
}
