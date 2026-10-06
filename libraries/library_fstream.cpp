// =============================================================================
//  C++ FILE I/O - (<fstream>)
// ============================================================================
//  Three stream classes:
//    std::ifstream  read only
//    std::ofstream  write only
//    std::fstream   read & write
//
//  Contents
//    1. Writing            (ofstream, check open, check write errors)
//    2. Reading            (line / word / char - and the eof() trap)
//    3. Appending          (ios::app)
//    4. Read + write       (fstream, seekg / seekp)
//    5. Binary I/O         (write / read)
//    6. Stream state flags (good, eof, fail, bad, clear)
//    7. Exceptions         (optional alternative to checking flags)
//    8. Gotchas
// =============================================================================

#include <cstdlib>      // EXIT_SUCCESS / EXIT_FAILURE
#include <fstream>
#include <iostream>
#include <string>

// NOTE: no `using namespace std;`. Spelling out std:: avoids name clashes and
// is the habit most codebases expect (especially never in headers).

// -----------------------------------------------------------------------------
// OPEN MODES (combine with |)
// -----------------------------------------------------------------------------
//   std::ios::in      open for reading
//   std::ios::out     open for writing (truncates existing content by default)
//   std::ios::app     every write goes to the END of the file
//   std::ios::ate     jump to the end on open (only the starting position;
//                     later writes can still go anywhere - use app to append)
//   std::ios::trunc   delete existing content
//   std::ios::binary  no newline translation (needed for raw bytes)
//
//   Defaults: ifstream = in, ofstream = out|trunc, fstream = in|out
//   (fstream with in|out does NOT create a missing file; the open fails.)

// -----------------------------------------------------------------------------
// 1. WRITING
// -----------------------------------------------------------------------------
bool writeLinesFromUser(const std::string& path, int count) {
    std::ofstream fout(path);              // constructor opens the file; truncates it
    if (!fout.is_open()) {                 // always check the open succeeded
        std::cerr << "Error: could not open " << path << " for writing\n";
        return false;
    }

    std::cout << "Enter " << count << " lines of text:\n";
    std::string line;
    // Putting getline in the loop condition stops cleanly if input ends early
    // (the original wrote blank lines forever after EOF on stdin).
    for (int i = 0; i < count && std::getline(std::cin, line); ++i) {
        fout << line << '\n';              // '\n' instead of std::endl (endl also flushes = slow)
    }

    return static_cast<bool>(fout);        // false if any write failed (e.g. disk full)
}   // fout closes + flushes automatically here (RAII), so close() is optional

// -----------------------------------------------------------------------------
// 2. READING
// -----------------------------------------------------------------------------
// Reading functions at a glance:
//   std::getline(in, s)  one line into a string (newline removed)
//   in >> x              formatted read, skips whitespace (word, int, double ...)
//   in.get(ch)           one char, including spaces and newlines
//   in.read(buf, n)      n raw bytes (binary)

void readByLine(const std::string& path) {
    std::ifstream in(path);
    if (!in.is_open()) {
        std::cerr << "Error: could not open " << path << " for reading\n";
        return;
    }
    std::string line;
    int lineNo = 1;
    while (std::getline(in, line)) {       // the read itself is the loop condition
        std::cout << "  " << lineNo++ << ": " << line << '\n';
    }
}

void readByWord(const std::string& path) {
    std::ifstream in(path);
    std::string word;
    int count = 0;
    while (in >> word) {                   // splits on any whitespace
        ++count;
    }
    std::cout << "  word count: " << count << '\n';
}

void readByChar(const std::string& path) {
    std::ifstream in(path);
    char ch;
    int chars = 0, newlines = 0;
    while (in.get(ch)) {                   // get() returns the stream; false at EOF
        ++chars;
        if (ch == '\n') ++newlines;
    }
    std::cout << "  chars: " << chars << ", newlines: " << newlines << '\n';

    // THE eof() TRAP - don't loop on it:
    //   while (!in.eof()) { in.get(ch); ... }   // WRONG
    // eof() only becomes true AFTER a read has already failed, so the loop body
    // runs once more with garbage (and an empty body, as in the original, would
    // loop forever on a file that failed to open). Test the read itself instead.
}

// -----------------------------------------------------------------------------
// 3. APPENDING
// -----------------------------------------------------------------------------
bool appendLine(const std::string& path, const std::string& text) {
    std::ofstream out(path, std::ios::app);    // keeps old content, writes at the end
    if (!out.is_open()) {
        return false;
    }
    out << text << '\n';
    return static_cast<bool>(out);
}

// -----------------------------------------------------------------------------
// 4. READ + WRITE WITH fstream
// -----------------------------------------------------------------------------
// Between switching from reading to writing (or back) you must seek (or flush).
// seekg = move the GET (read) position, seekp = move the PUT (write) position.
void fstreamDemo(const std::string& path) {
    std::fstream f(path, std::ios::in | std::ios::out);   // file must already exist
    if (!f.is_open()) {
        std::cerr << "Error: could not open " << path << " for read/write\n";
        return;
    }

    std::string first;
    std::getline(f, first);
    std::cout << "  first line: " << first << '\n';

    f.clear();                              // clear any eof/fail flags before seeking
    f.seekp(0, std::ios::end);              // jump to end, then write
    f << "added via fstream\n";

    f.seekg(0);                             // back to the start, then read everything
    std::string line;
    while (std::getline(f, line)) {
        std::cout << "  | " << line << '\n';
    }
}

// -----------------------------------------------------------------------------
// 5. BINARY I/O  (write() / read() work on raw bytes, not text)
// -----------------------------------------------------------------------------
void binaryDemo(const std::string& path) {
    const int data[3] = {10, 20, 30};
    {
        std::ofstream out(path, std::ios::binary);
        out.write(reinterpret_cast<const char*>(data), sizeof(data));
    }                                       // scope ends: file closed and flushed

    int back[3] = {};
    std::ifstream in(path, std::ios::binary);
    in.read(reinterpret_cast<char*>(back), sizeof(back));
    std::cout << "  read " << in.gcount() << " bytes: "
              << back[0] << ' ' << back[1] << ' ' << back[2] << '\n';
    // Raw dumps depend on type size / endianness, so they are not portable
    // between machines. Fine for a demo; use a defined format for real data.
}

// -----------------------------------------------------------------------------
// 6. STREAM STATE FLAGS
// -----------------------------------------------------------------------------
//   good()  no error flags set
//   eof()   a read hit the end of the file
//   fail()  a logical error (bad format, failed open) - also true after EOF reads
//   bad()   serious I/O error (hardware, corrupted stream)
//   is_open()  is a file attached?
//   clear() reset the flags so the stream can be used again
void printState(const char* label, const std::ios& s) {
    std::cout << "  " << label << ": good=" << s.good() << " eof=" << s.eof()
              << " fail=" << s.fail() << " bad=" << s.bad() << '\n';
}

void stateDemo(const std::string& path) {
    std::cout << std::boolalpha;

    std::ifstream missing("does_not_exist.txt");
    std::cout << "  missing file is_open=" << missing.is_open() << '\n';
    printState("missing", missing);

    std::ifstream in(path);
    std::string line;
    while (std::getline(in, line)) {}
    printState("after reading all", in);    // eof and fail are both set

    in.clear();
    printState("after clear()", in);        // flags reset (position is still the end)

    std::cout << std::noboolalpha;
}

// -----------------------------------------------------------------------------
// 7. EXCEPTIONS (optional: instead of checking flags after each operation)
// -----------------------------------------------------------------------------
void exceptionDemo() {
    std::ifstream in;
    in.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        in.open("does_not_exist.txt");      // throws because the open fails
    } catch (const std::ios_base::failure& e) {
        std::cout << "  caught std::ios_base::failure: " << e.what() << '\n';
    }
    // Careful: with failbit enabled, a getline loop will THROW when it reaches
    // EOF. Most code sticks to checking return values, as in sections 1-4.
}

// -----------------------------------------------------------------------------
// 8. GOTCHAS
// -----------------------------------------------------------------------------
//  * Check is_open() (or `if (!stream)`) right after opening.
//  * Loop on the read itself: while (getline(...)) / while (in >> x) / while (in.get(c)),
//    never while (!in.eof()).
//  * Once a stream reaches EOF, further reads fail until you clear() and seekg(0).
//    (Reading by line, then by word on the SAME stream, gives nothing the 2nd time.)
//  * Variables in one scope need unique names: declaring `ifstream inFile`
//    twice in the same function is a compile error. Use separate functions/scopes.
//  * Constructing an ofstream truncates an existing file; use ios::app to keep it.
//  * Relative paths resolve against the working directory, not the source file.
//  * Missing directories are not created for you.
//  * Don't forget semicolons (`return 1;`).

// -----------------------------------------------------------------------------
// DEMO
// -----------------------------------------------------------------------------
int main() {
    const std::string path = "NewFile.txt";

    if (!writeLinesFromUser(path, 5)) {
        return EXIT_FAILURE;
    }
    std::cout << "Text successfully written to " << path << "\n\n";

    std::cout << "[read by line]\n";   readByLine(path);
    std::cout << "[read by word]\n";   readByWord(path);
    std::cout << "[read by char]\n";   readByChar(path);

    std::cout << "[append]\n";
    if (appendLine(path, "appended line")) {
        readByLine(path);
    }

    std::cout << "[fstream read + write]\n";   fstreamDemo(path);
    std::cout << "[binary]\n";                 binaryDemo("numbers.bin");
    std::cout << "[state flags]\n";            stateDemo(path);
    std::cout << "[exceptions]\n";             exceptionDemo();

    return 0;
}
