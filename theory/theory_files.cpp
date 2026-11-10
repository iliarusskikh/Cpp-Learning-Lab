#include <fstream>
#include <iostream>
#include <string>
#include <limits>

int main() {
    const std::string filename = "sample.txt";

    // 1. Initial Creation / Overwrite Mode
    {
        std::ofstream out(filename);
        if (!out) {
            std::cerr << "Error: Ultimate failure creating " << filename << "\n";
            return 1;
        }
        out << "hello file\nsecond line\n";
    } // 'out' closes automatically here

    // 2. Append Mode: Let the user add their own lines dynamically
    {
        // std::ios::app ensures text is added to the end without deleting old content
        std::ofstream append_out(filename, std::ios::app);
        if (!append_out) {
            std::cerr << "Error: Could not open " << filename << " to append data.\n";
            return 1;
        }

        std::cout << "Enter a line of text to add to the file: ";
        std::string user_input;
        std::getline(std::cin, user_input);

        // Append user input with a trailing newline
        append_out << user_input << '\n';
        std::cout << "Successfully added your text to " << filename << "!\n\n";
    }

    // 3. Read & Display Mode: Read the final content line-by-line
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Error: Could not open " << filename << " for reading.\n";
        return 1;
    }

    std::cout << "--- Current Contents of " << filename << " ---\n";
    std::string line;
    int line_number = 1;

    while (std::getline(in, line)) {
        // Formats the output nicely with line numbers
        std::cout << line_number << ": " << line << '\n';
        line_number++;
    }
    std::cout << "-----------------------------------\n";

    return 0;
}
