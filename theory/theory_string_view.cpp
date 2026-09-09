#include <iostream>
#include <string>
#include <string_view>
#include <chrono>
#include <vector>

// ============================================
// BENEFIT 1: ZERO-COPY SUBSTRING OPERATIONS
// ============================================

// BAD: std::string creates copies
std::string getSubstringString(const std::string& s, size_t start, size_t len) {
    return s.substr(start, len);  // ALLOCATES NEW MEMORY + COPIES DATA
}

// GOOD: std::string_view just points to existing data
std::string_view getSubstringView(std::string_view s, size_t start, size_t len) {
    return s.substr(start, len);    // O(1) - just adjusts pointer + length!
}

// ============================================
// BENEFIT 2: NO HEAP ALLOCATION FOR LITERALS
// ============================================

// BAD: Creates heap allocation even for compile-time known strings
void processString(const std::string& s) {
    std::cout << "String (copied): " << s << "\n";
}

// GOOD: Zero overhead for string literals
void processStringView(std::string_view sv) {
    std::cout << "StringView (no copy): " << sv << "\n";
}

// ============================================
// BENEFIT 3: UNIVERSAL ACCEPTOR (works with string, char*, literals)
// ============================================

void universalAcceptor(std::string_view sv) {
    std::cout << "Received: " << sv << " (length: " << sv.length() << ")\n";
}

// ============================================
// BENEFIT 4: EFFICIENT PARSING / TOKENIZATION
// ============================================

std::vector<std::string_view> splitView(std::string_view str, char delimiter) {
    std::vector<std::string_view> tokens;
    size_t start = 0;
    
    while (true) {
        size_t end = str.find(delimiter, start);
        if (end == std::string_view::npos) {
            tokens.push_back(str.substr(start));
            break;
        }
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
    }
    return tokens;  // No copies of the actual string data!
}

// BAD VERSION: Creates copies of every token
std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    size_t start = 0;
    
    while (true) {
        size_t end = str.find(delimiter, start);
        if (end == std::string::npos) {
            tokens.push_back(str.substr(start));  // COPY!
            break;
        }
        tokens.push_back(str.substr(start, end - start));  // COPY!
        start = end + 1;
    }
    return tokens;
}

// ============================================
// BENEFIT 5: REMOVING PREFIX/SUFFIX WITHOUT COPY
// ============================================

std::string_view trimView(std::string_view sv) {
    // Remove leading whitespace
    while (!sv.empty() && std::isspace(sv.front())) {
        sv.remove_prefix(1);  // O(1)! Just moves pointer forward
    }
    // Remove trailing whitespace
    while (!sv.empty() && std::isspace(sv.back())) {
        sv.remove_suffix(1);  // O(1)! Just reduces length
    }
    return sv;
}

// ============================================
// PERFORMANCE BENCHMARK
// ============================================

template<typename Func>
void benchmark(const char* name, Func f, int iterations) {
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i) {
        f();
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << name << ": " << ms << "ms\n";
}

int main() {
    std::cout << "=== std::string_view vs std::string Demo ===\n\n";
    
    // -------------------------------------------------
    // Demo 1: Substring performance
    // -------------------------------------------------
    std::cout << "--- Substring Operations ---\n";
    std::string largeString = "Hello, this is a very large string that we want to take a small piece from";
    
    auto subStr = getSubstringString(largeString, 0, 5);      // Copies "Hello"
    auto subView = getSubstringView(largeString, 0, 5);       // Just points to "Hello"
    
    std::cout << "String copy: " << subStr << " (new allocation)\n";
    std::cout << "String view: " << subView << " (zero-copy)\n";
    
    // -------------------------------------------------
    // Demo 2: Universal acceptor
    // -------------------------------------------------
    std::cout << "\n--- Universal Acceptor ---\n";
    std::string stdStr = "std::string";
    const char* cStr = "C-string";
    
    universalAcceptor(stdStr);        // Works with std::string
    universalAcceptor(cStr);          // Works with const char*
    universalAcceptor("literal");     // Works with string literals
    universalAcceptor(std::string_view(cStr).substr(2, 7));  // Works with other views
    
    // -------------------------------------------------
    // Demo 3: Efficient tokenization
    // -------------------------------------------------
    std::cout << "\n--- Tokenization (Split) ---\n";
    std::string csv = "apple,banana,cherry,date,elderberry,fig,grape";
    
    auto tokensView = splitView(csv, ',');
    std::cout << "StringView tokens (" << tokensView.size() << "): ";
    for (auto t : tokensView) std::cout << "[" << t << "] ";
    std::cout << "\n";
    
    // -------------------------------------------------
    // Demo 4: Non-modifying trim
    // -------------------------------------------------
    std::cout << "\n--- Trim Without Copy ---\n";
    std::string messy = "   \t  lots of whitespace   \n  ";
    std::string_view clean = trimView(messy);
    std::cout << "Original: \"" << messy << "\"\n";
    std::cout << "Trimmed:  \"" << clean << "\"\n";
    std::cout << "Original still intact: \"" << messy << "\"\n";
    
    // -------------------------------------------------
    // Demo 5: Performance comparison
    // -------------------------------------------------
    std::cout << "\n--- Performance Benchmark ---\n";
    const int ITERATIONS = 1000000;
    std::string testStr = "The quick brown fox jumps over the lazy dog";
    
    benchmark("std::string substr", [&]() {
        volatile auto s = testStr.substr(4, 5);  // "quick"
    }, ITERATIONS);
    
    benchmark("std::string_view substr", [&]() {
        volatile auto sv = std::string_view(testStr).substr(4, 5);
    }, ITERATIONS);
    
    // -------------------------------------------------
    // IMPORTANT: Lifetime considerations
    // -------------------------------------------------
    std::cout << "\n--- ⚠️  Lifetime Warning ---\n";
    std::string_view dangling;
    {
        std::string temporary = "I go out of scope";
        dangling = temporary;  // DANGEROUS: points to temporary!
        std::cout << "Inside scope: " << dangling << "\n";
    }
    // std::cout << dangling;  // UNDEFINED BEHAVIOR! Don't do this!
    std::cout << "dangling is now invalid (commented out to prevent crash)\n";
    
    // SAFE approach:
    std::string owner = "I persist";
    std::string_view safeView = owner;  // OK: owner outlives view
    std::cout << "Safe view: " << safeView << "\n";
    
    // -------------------------------------------------
    // Demo 6: Comparison operations
    // -------------------------------------------------
    std::cout << "\n--- Comparison & Search ---\n";
    std::string_view text = "Hello World";
    std::cout << "starts_with('Hello'): " << text.starts_with("Hello") << "\n";
    std::cout << "ends_with('World'): " << text.ends_with("World") << "\n";
    std::cout << "contains('lo Wo'): " << text.contains("lo Wo") << "\n";  // C++23
    
    // find operations (same API as string)
    auto pos = text.find("World");
    if (pos != std::string_view::npos) {
        std::cout << "'World' found at position: " << pos << "\n";
    }
    
    // -------------------------------------------------
    // Demo 7: Conversion back to string (when needed)
    // -------------------------------------------------
    std::cout << "\n--- Conversion to std::string ---\n";
    std::string_view view = "Make me owned";
    std::string owned(view);  // Explicit copy when you need ownership
    std::cout << "Explicit copy: " << owned << "\n";
    
    // Or using std::string constructor explicitly
    std::string anotherOwned{view};
    
    
    /*
    std::string input = "AAPL,150.25,100;GOOG,2800.50,50;MSFT,300.75,200";
    std::string_view input_view(input);
    
    size_t pos = 0;
    //std::cout << "Input string: " << input_view.substr(0, input_view.find(';')) << "\n";
    while((pos = input_view.find(';')) != std::string_view::npos) {
        std::string_view token = input_view.substr(0, pos);
        std::cout << "Token: " << token << "\n";
        input_view.remove_prefix(pos + 1); // Directly use pos instead of recalculating token.size()
        //input_view.remove_prefix(token.size() + 1); // +1 to skip the semicolon
    }
    // Handle the last token after the last semicolon
    if(!input_view.empty()) {
        std::cout << "Token: " << input_view << "\n";
    }
*/
    
    
    return 0;
}
