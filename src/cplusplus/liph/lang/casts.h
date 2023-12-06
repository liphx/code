#ifndef LIPH_LANG_CASTS_H_
#define LIPH_LANG_CASTS_H_

#include <cassert>
#include <cstring>
#include <type_traits>

namespace liph {

template <typename To, typename From>
inline To implicit_cast(const From& f) {
    return f;
}

template <typename To, typename From>
inline To down_cast(From *f) {
    if (false) {
        implicit_cast<From *, To>(0);
    }
#if !defined(NDEBUG)
    assert(f == nullptr || dynamic_cast<To>(f) != nullptr);
#endif
    return static_cast<To>(f);
}

template <typename To, typename From>
inline To down_cast(From& f) {
    using Pointer = typename std::remove_reference<To>::type *;
    if (false) {
        implicit_cast<From *, Pointer>(0);
    }
#if !defined(NDEBUG)
    assert(dynamic_cast<Pointer>(&f) != nullptr);
#endif
    return *static_cast<Pointer>(&f);
}

template <typename To, typename From>
inline To bit_cast(const From& from) {
    static_assert(sizeof(From) == sizeof(To), "bit_cast_with_different_sizes");
    To dest;
    memcpy(&dest, &from, sizeof(dest));
    return dest;
}

}  // namespace liph

#endif  // LIPH_LANG_CASTS_H_
