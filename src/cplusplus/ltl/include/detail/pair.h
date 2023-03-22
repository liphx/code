#ifndef LTL_DETAIL_PAIR_H_
#define LTL_DETAIL_PAIR_H_

#include <functional>  // std::unwrap_ref_decay_t

#include "utility"

namespace ltl {

template <class T1, class T2>
struct pair {
    typedef T1 first_type;
    typedef T2 second_type;

    T1 first;
    T2 second;

    constexpr pair() : first(), second() {}
    constexpr pair(const T1& x, const T2& y) : first(x), second(y) {}
    constexpr pair& operator=(const pair& other) {
        first = other.first;
        second = other.second;
        return *this;
    }

    constexpr pair(const pair&) = default;
    constexpr pair(pair&&) = default;
};

template <class T1, class T2>
constexpr pair<std::unwrap_ref_decay_t<T1>, std::unwrap_ref_decay_t<T2>> make_pair(T1&& x, T2&& y) {
    return pair<std::unwrap_ref_decay_t<T1>, std::unwrap_ref_decay_t<T2>>(ltl::forward<T1>(x), ltl::forward<T2>(y));
}

}  // namespace ltl

#endif  // LTL_DETAIL_PAIR_H_
