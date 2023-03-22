#ifndef LTL_DETAIL_ITERATOR_H_
#define LTL_DETAIL_ITERATOR_H_

#include <iterator>

#include "type_traits"

namespace ltl {

// struct output_iterator_tag {};
// struct input_iterator_tag {};
// struct forward_iterator_tag : public input_iterator_tag {};
// struct bidirectional_iterator_tag : public forward_iterator_tag {};
// struct random_access_iterator_tag : public bidirectional_iterator_tag {};
// struct contiguous_iterator_tag : public random_access_iterator_tag {};

using std::output_iterator_tag;
using std::input_iterator_tag;
using std::forward_iterator_tag;
using std::bidirectional_iterator_tag;
using std::random_access_iterator_tag;
using std::contiguous_iterator_tag;

template <class I>
struct iterator_traits {
    using iterator_category = typename I::iterator_category;
    using value_type = typename I::value_type;
    using difference_type = typename I::difference_type;
    using pointer = typename I::pointer;
    using reference = typename I::reference;
};

template <class T>
struct iterator_traits<T *> {
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = T *;
    using reference = remove_cv_t<T>;
};

template <class InputIterator, class Distance>
constexpr void advance(InputIterator& i, Distance n, input_iterator_tag) {
    while (n-- > 0) {
        ++i;
    }
}

template <class InputIterator, class Distance>
constexpr void advance(InputIterator& i, Distance n, bidirectional_iterator_tag) {
    if (n > 0) {
        while (n-- > 0) {
            ++i;
        }
    } else {
        while (n++ > 0) {
            --i;
        }
    }
}

template <class InputIterator, class Distance>
constexpr void advance(InputIterator& i, Distance n, random_access_iterator_tag) {
    i += n;
}

template <class InputIterator, class Distance>
constexpr void advance(InputIterator& i, Distance n) {
    return advance(i, n, typename iterator_traits<InputIterator>::iterator_category());
}

template <class InputIterator>
constexpr InputIterator next(InputIterator x, typename iterator_traits<InputIterator>::difference_type n = 1) {
    advance(x, n);
    return x;
}

template <class BidirectionalIterator>
constexpr BidirectionalIterator prev(
        BidirectionalIterator x, typename iterator_traits<BidirectionalIterator>::difference_type n = 1) {
    advance(x, -n);
    return x;
}

template <class InputIterator>
constexpr typename iterator_traits<InputIterator>::difference_type distance(
        InputIterator first, InputIterator last, input_iterator_tag) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first++ != last) {
        ++n;
    }
    return n;
}

template <class InputIterator>
constexpr typename iterator_traits<InputIterator>::difference_type distance(
        InputIterator first, InputIterator last, random_access_iterator_tag) {
    return last - first;
}

template <class InputIterator>
constexpr typename iterator_traits<InputIterator>::difference_type distance(InputIterator first, InputIterator last) {
    return distance(first, last, typename iterator_traits<InputIterator>::iterator_category());
}

}  // namespace ltl

#endif  // LTL_DETAIL_ITERATOR_H_
