#include <cstddef>
#include <iostream>

#define print(s) std::cout << #s " = " << s << std::endl
#define psize(t) print(sizeof(t))

int main() {
    // sizeof(void);
    // invalid application of 'sizeof' to an incomplete type 'void'

    psize(bool);
    psize(void *);
    psize(std::nullptr_t);
    psize(std::size_t);     // at least 16 bits.
    psize(std::ptrdiff_t);  // at least 17 bits.
    psize(std::max_align_t);

    // 1 == sizeof(char) <= sizeof(short) <= sizeof(int) <= sizeof(long) <= sizeof(long long)
    psize(char);       // 1
    psize(short);      // at least 16 bits.
    psize(int);        // at least 16 bits.
    psize(long);       // at least 32 bits.
    psize(long long);  // at least 64 bits.
    psize(std::byte);

    psize(float);
    psize(double);
    psize(long double);
}
