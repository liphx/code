#ifndef LIPH_HASH_H_
#define LIPH_HASH_H_

#include <cstdint>

namespace liph {

inline unsigned bkdr_hash(const char *str) {
    if (str == nullptr) return 0U;
    unsigned hashval;
    static const unsigned seed = 31;
    for (hashval = 0; *str != '\0'; str++) {
        hashval = hashval * seed + *str;
    }
    return hashval;
}

inline uint64_t bkdr_hash64(const char *str) {
    if (str == nullptr) return 0U;
    union {
        uint32_t u32[2];
        uint64_t u64;
    };
    static const uint32_t seeds[2] = {13, 131};
    for (u32[0] = 0, u32[1] = 0; *str != '\0'; str++) {
        u32[0] = u32[0] * seeds[0] + *str;
        u32[1] = u32[1] * seeds[1] + *str;
    }
    return u64;
}

}  // namespace liph

#endif  // LIPH_HASH_H_
