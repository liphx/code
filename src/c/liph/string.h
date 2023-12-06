#ifndef LIPH_STRING_H_
#define LIPH_STRING_H_

#include <stddef.h>  // size_t
#include <string.h>

/// return strlen(prefix) if str starts with prefix, else 0
static inline size_t starts_with(const char *str, const char *prefix) {
    size_t len = strlen(prefix);
    return strncmp(str, prefix, len) == 0 ? len : 0;
}

#endif  // LIPH_STRING_H_
