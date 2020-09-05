#include <stdint.h>
#include <stdio.h>

int main()
{
    printf("%d\t", sizeof(int8_t));
    printf("%d\t", sizeof(int64_t));
    printf("%d\t", sizeof(uint8_t));
    printf("%d\n", sizeof(uint64_t));
    // 1 8 1 8

    int8_t i8 = 128;
    uint8_t ui8 = i8;
    printf("%d, %d\n", i8, ui8); // -128, 128

    

    return 0;
}