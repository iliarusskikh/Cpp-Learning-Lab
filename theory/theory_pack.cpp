#pragma pack(push, 1)
struct PackedBadLayout {
    char a;   // offset 0
    int  b;   // offset 1  <-- not 4-byte aligned!
    char c;   // offset 5
    int  d;   // offset 6  <-- not 4-byte aligned!
    char e;   // offset 10
};
#pragma pack(pop)
// sizeof(PackedBadLayout) == 11, but 'b' and 'd' sit at unaligned addresses

int main() {
    PackedBadLayout obj{};

    // Taking the address of 'b' and treating it as int*
    int* pb = &obj.b;   // address = &obj + 1, NOT a multiple of 4

    printf("&obj    = %p\n", (void*)&obj);
    printf("&obj.b  = %p\n", (void*)pb);

    *pb = 0x11223344;   // write through an unaligned int*

    // On x86-64 this often "works" silently (slower, extra bus cycles)
    // On ARM/embedded targets this can:
    //   - trigger a bus error / hardware fault (SIGBUS), or
    //   - the compiler may quietly split it into byte-wise loads/stores,
    //     losing any assumption of atomicity
    // Either way, dereferencing a misaligned int* is undefined behavior
    // in C++, even if it "happens to work" on your machine.

    printf("obj.b   = 0x%08X\n", *pb);
}

//so pack only used when byte restrictive transmission
/*
 
 #include <cstdio>
 #include <cstddef>  // offsetof

 struct Example {
     char a;   // 1 byte
     int  b;   // 4 bytes
     char c;   // 1 byte
     int  d;   // 4 bytes
     char e;   // 1 byte
 };

 int main() {
     printf("sizeof(Example) = %zu\n\n", sizeof(Example));

     printf("offset of a = %zu\n", offsetof(Example, a));
     printf("offset of b = %zu\n", offsetof(Example, b));
     printf("offset of c = %zu\n", offsetof(Example, c));
     printf("offset of d = %zu\n", offsetof(Example, d));
     printf("offset of e = %zu\n", offsetof(Example, e));
 }
 
 */


/*
 
 #include <cstdio>
 #include <cstddef>   // std::byte
 #include <cstring>   // std::memcpy

 struct Example {
     char a;   // offset 0
     int  b;   // offset 4
     char c;   // offset 8
     int  d;   // offset 12
     char e;   // offset 16
 };            // sizeof == 20

 int main() {
     Example ex{};
     ex.a = 'A';
     ex.b = 0x11111111;
     ex.c = 'C';
     ex.d = 0x22222222;
     ex.e = 'E';

     // View the struct's raw memory as std::byte
     auto* raw = reinterpret_cast<std::byte*>(&ex);

     printf("sizeof(Example) = %zu\n\n", sizeof(Example));
     printf("Raw bytes:\n");
     for (size_t i = 0; i < sizeof(Example); ++i) {
         printf("  [%2zu] = 0x%02X\n", i, std::to_integer<int>(raw[i]));
     }
 }
 
 
 #include <cstddef>
 #include <array>

 int main() {
     // A raw 8-byte buffer, not tied to any particular type
     std::array<std::byte, 8> buffer{};

     // Manually place a 4-byte int at offset 0, and a char at offset 4
     int value = 0x1234; //2byte number
     std::memcpy(buffer.data(), &value, sizeof(value));
     buffer[4] = std::byte{'X'};

     for (auto b : buffer) {
         printf("%02X ", std::to_integer<int>(b));
     }
     printf("\n");
 }
 */
