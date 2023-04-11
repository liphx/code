#include <bit>

#include "liph/liph.h"

// 0x00 0x01 0x02 0x03
// 0 	0 	 0    1 		big endians
// 1 	0 	 0    0 		little endians

bool little_endians() {
    static union {
        int a;
        char b;
    } u{.a = 1};
    return u.b;
}

bool little_endians2() {
    static int i = 1;
    return *(char *)&i == 1;
}

bool little_endians3() { return std::endian::native == std::endian::little; }

int main() {
    D(little_endians());
    D(little_endians2());
    D(little_endians3());
}
