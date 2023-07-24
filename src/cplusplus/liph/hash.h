#ifndef LIPH_HASH_H_
#define LIPH_HASH_H_

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

}  // namespace liph

#endif  // LIPH_HASH_H_
