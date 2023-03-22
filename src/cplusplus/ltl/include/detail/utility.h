#ifndef LTL_DETAIL_UTILITY_H_
#define LTL_DETAIL_UTILITY_H_

#include "detail/type_traits.h"

/*
 * forward
 * move
 * swap
 */

namespace ltl {

template <class T>
constexpr T&& forward(remove_reference_t<T>& t) noexcept {
    return static_cast<T&&>(t);
}

template <class T>
constexpr T&& forward(remove_reference_t<T>&& t) noexcept {
    return static_cast<T&&>(t);
}

template <class T>
constexpr remove_reference_t<T>&& move(T&& t) noexcept {
    return static_cast<remove_reference_t<T>&&>(t);
}

template <class T>
constexpr void swap(T& a, T& b) noexcept {
    T tmp = move(a);
    a = move(b);
    b = move(tmp);
}

}  // namespace ltl

#endif  // LTL_DETAIL_UTILITY_H_
