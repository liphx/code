#ifndef LTL_DETAIL_ALGORITHM_H_
#define LTL_DETAIL_ALGORITHM_H_

/*
 * all_of
 * any_of
 * none_of
 * for_each
 * is_sorted_until
 * is_sorted
 * lower_bound
 * upper_bound
 * binary_search
 * equal_range
 */

#include <functional>  // less

#include "iterator"
#include "utility"

namespace ltl {

template <class InputIt, class UnaryPredicate>
constexpr bool all_of(InputIt first, InputIt last, UnaryPredicate p) {
    for (; first != last; ++first) {
        if (!p(*first)) return false;
    }
    return true;
}

template <class InputIt, class UnaryPredicate>
constexpr bool any_of(InputIt first, InputIt last, UnaryPredicate p) {
    for (; first != last; ++first) {
        if (p(*first)) return true;
    }
    return false;
}

template <class InputIt, class UnaryPredicate>
constexpr bool none_of(InputIt first, InputIt last, UnaryPredicate p) {
    for (; first != last; ++first) {
        if (p(*first)) return false;
    }
    return true;
}

template <class InputIt, class UnaryFunction>
constexpr UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f) {
    for (; first != last; ++first) {
        f(*first);
    }
    return f;
}

/*
 * 检验范围 [first, last) ，并寻找始于 first 且其中元素已以不降序排序的最大范围。
 * 返回始于 first 且其中元素已以升序排序的最大范围。即满足范围 [first, it) 已排序的最后迭代器 it 。
 */
template <class ForwardIt, class Compare = std::less<typename iterator_traits<ForwardIt>::value_type>>
ForwardIt is_sorted_until(ForwardIt first, ForwardIt last, Compare comp = Compare()) {
    while (first != last) {
        ForwardIt prev = first;
        first++;
        if (first != last && !comp(*prev, *first)) return first;
    }
    return last;
}

/*
 * 检查 [first, last) 中的元素是否以不降序排序
 */
template <class ForwardIt, class Compare = std::less<typename iterator_traits<ForwardIt>::value_type>>
inline bool is_sorted(ForwardIt first, ForwardIt last, Compare comp = Compare()) {
    return ltl::is_sorted_until(first, last, comp) == last;
}

template <class ForwardIt, class T, class Compare>
constexpr ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp) {
    while (first != last) {
        auto it = first;
        auto n = distance(first, last);
        advance(it, n / 2);
        if (comp(*it, value)) {
            first = ++it;
        } else {
            last = it;
        }
    }
    return first;
}

template <class ForwardIt, class T>
constexpr ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value) {
    return ltl::lower_bound(first, last, value, std::less<typename iterator_traits<ForwardIt>::value_type>());
}

template <class ForwardIt, class T, class Compare>
constexpr ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp) {
    while (first != last) {
        auto it = first;
        auto n = distance(first, last);
        advance(it, n / 2);
        if (!comp(value, *it)) {
            first = ++it;
        } else {
            last = it;
        }
    }
    return first;
}

template <class ForwardIt, class T>
constexpr ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value) {
    return ltl::upper_bound(first, last, value, std::less<typename iterator_traits<ForwardIt>::value_type>());
}

template <class ForwardIt, class T, class Compare>
constexpr bool binary_search(ForwardIt first, ForwardIt last, const T& value, Compare comp) {
    auto it = ltl::lower_bound(first, last, value, comp);
    return (!(it == last) && !comp(value, *it));
}

template <class ForwardIt, class T>
constexpr bool binary_search(ForwardIt first, ForwardIt last, const T& value) {
    return ltl::binary_search(first, last, value, std::less<typename iterator_traits<ForwardIt>::value_type>());
}

template <class ForwardIt, class T, class Compare>
constexpr pair<ForwardIt, ForwardIt> equal_range(ForwardIt first, ForwardIt last, const T& value, Compare comp) {
    return make_pair(ltl::lower_bound(first, last, value, comp), ltl::upper_bound(first, last, value, comp));
}

template <class ForwardIt, class T>
constexpr pair<ForwardIt, ForwardIt> equal_range(ForwardIt first, ForwardIt last, const T& value) {
    return ltl::equal_range(first, last, value, std::less<typename iterator_traits<ForwardIt>::value_type>());
}

}  // namespace ltl

#endif  // LTL_DETAIL_ALGORITHM_H_
