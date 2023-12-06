/*
 *  int8_t
 *  int16_t
 *  int32_t
 *  int64_t
 *  uint8_t
 *  uint16_t
 *  uint32_t
 *  uint64_t
 **
 *  int_fast8_t
 *  int_fast16_t
 *  int_fast32_t
 *  int_fast64_t
 *  uint_fast8_t
 *  uint_fast16_t
 *  uint_fast32_t
 *  uint_fast64_t
 **
 *  int_least8_t
 *  int_least16_t
 *  int_least32_t
 *  int_least64_t
 *  uint_least8_t
 *  uint_least16_t
 *  uint_least32_t
 *  uint_least64_t
 **
 *  intmax_t
 *  uintmax_t
 **
 *  intptr_t
 *  uintptr_t
 *
 *  INT8_MIN, INTMAX_MIN, UINT32_MAX, UINTPTR_MAX, ...
 *
 *  INT8_C
 *  INT16_C
 *  INT32_C
 *  INT64_C
 *  INTMAX_C
 *  UINT8_C
 *  UINT16_C
 *  UINT32_C
 *  UINT64_C
 *  UINTMAX_C
 */

#include <cstdint>
#include <iostream>

int main() {
    std::cout << sizeof(int8_t) << " " << sizeof(int64_t) << " " << sizeof(uint8_t) << " " << sizeof(uint64_t)
              << std::endl;
    // 1 8 1 8
    int8_t i8 = static_cast<int8_t>(128);
    uint8_t ui8 = static_cast<uint8_t>(i8);
    std::cout << static_cast<int>(i8) << " " << static_cast<int>(ui8) << std::endl;  // -128, 128
}
