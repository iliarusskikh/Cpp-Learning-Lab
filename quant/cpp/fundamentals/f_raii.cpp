#include <iostream>

int main(){
    try {
        FileHandler myFile("example.txt", std::ios::out);
        myFile.get_file_stream() << "Hello, John!" << std:: endl;
        // the file is automatically closed when myFile goes out of scope
    } catch (const std::exceptions & e){
        std::cerr << "Error: "<< e.what() << std::endl;
    }
    return 0;
}
