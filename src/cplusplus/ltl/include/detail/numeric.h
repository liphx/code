#ifndef LTL_DETAIL_NUMERIC_H_
#define LTL_DETAIL_NUMERIC_H_

#include "type_traits"  // std::common_type

/*
 * accumulate
 * iota
 * gcd
 * lcm
 */

namespace ltl {

template <class InputIt, class T>
constexpr T accumulate(InputIt first, InputIt last, T init) {
    for (; first != last; ++first) {
        init += *first;
    }
    return init;
}

template <class InputIt, class T, class BinaryOperation>
constexpr T accumulate(InputIt first, InputIt last, T init, BinaryOperation op) {
    for (; first != last; ++first) {
        init = op(init, *first);
    }
    return init;
}

template <class ForwardIt, class T>
constexpr void iota(ForwardIt first, ForwardIt last, T value) {
    for (; first != last; ++first) {
        *first = value;
        ++value;
    }
}

template <class M, class N>
constexpr std::common_type_t<M, N> gcd(M m, N n) {
    return m == 0 ? n : n == 0 ? m : gcd(n, m % n);
}

template <class M, class N>
constexpr std::common_type_t<M, N> lcm(M m, N n) {
    return (m != 0 && n != 0) ? (m / gcd(m, n)) * n : 0;
}

}  // namespace ltl

#endif  // LTL_DETAIL_NUMERIC_H_
