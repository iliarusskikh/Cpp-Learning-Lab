#include <charconv>
#include <fstream>
#include <iostream>
#include <queue>
#include <sstream>
#include <string>
#include <system_error>

struct MyOrder {
    std::string symbol;
    int quantity;
};

constexpr int kSymbolCol   = 3;
constexpr int kQuantityCol = 6;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Insufficient arguments passed.\n";
        return 1;
    }

    const std::string filepath = argv[1];
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Cannot open the file: " << filepath << '\n';
        return 1;
    }

    std::string line;
    if (!std::getline(file, line)) {  // skip header
        std::cerr << "File is empty (no header found).\n";
        return 1;
    }

    std::queue<MyOrder> orders;

    while (std::getline(file, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();  // handle CRLF files
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string token;
        std::string symbol;
        int quantity = 0;
        bool haveSymbol = false, haveQuantity = false;
        int col = 0;

        while (std::getline(ss, token, ',')) {
            if (col == kSymbolCol) {
                symbol = token;
                haveSymbol = true;
            } else if (col == kQuantityCol) {
                const char* first = token.data();
                const char* last  = first + token.size();
                auto [ptr, ec] = std::from_chars(first, last, quantity);
                if (ec != std::errc{} || ptr != last) {
                    std::cerr << "Conversion error for quantity: '" << token << "'\n";
                } else {
                    haveQuantity = true;
                }
            }
            ++col;
        }

        if (!haveSymbol || !haveQuantity) {
            std::cerr << "Skipping malformed row: " << line << '\n';
            continue;
        }
        orders.push({symbol, quantity});
    }

    file.close();

    std::cout << "Successfully queued " << orders.size() << " order(s):\n\n";

    while (!orders.empty()) {
        const MyOrder& current = orders.front();
        std::cout << "Symbol: " << current.symbol << " | Quantity: " << current.quantity << '\n';
        orders.pop();
    }
    return 0;
}
