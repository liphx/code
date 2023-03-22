#ifndef LTL_DETAIL_HEAP_H_
#define LTL_DETAIL_HEAP_H_

#include "iterator"

namespace ltl {

/*
 * 检查范围 [first, last) 并寻找始于 first 且为最大堆的最大范围
 * 使用 comp 比较元素
 * 返回始于 first 且为最大堆的最大范围上界。即满足范围 [first, it) 是最大堆的最末迭代器 it
 */
template <class RandomIt, class Compare = std::less<typename iterator_traits<RandomIt>::value_type>>
RandomIt is_heap_until(RandomIt first, RandomIt last, Compare comp = Compare()) {
    using Distance = typename iterator_traits<RandomIt>::difference_type;
    Distance n = distance(first, last);
    Distance parent = 0;
    for (Distance child = 1; child < n; child++) {
        if (comp(*(first + parent), *(first + child))) return first + child;
        if ((child & 1) == 0) parent++;
    }
    return last;
}

/*
 * 检查范围 [first, last) 中的元素是否为最大堆
 * 使用 comp 比较元素
 * 若范围为最大堆则为 true, 否则为 false
 */
template <class RandomIt, class Compare = std::less<typename iterator_traits<RandomIt>::value_type>>
inline bool is_heap(RandomIt first, RandomIt last, Compare comp = Compare()) {
    return ltl::is_heap_until(first, last, comp) == last;
}

/*
 * 堆调整
 */
template <class RandomIt, class Distance, class Compare = std::less<typename iterator_traits<RandomIt>::value_type>>
void adjust_heap(RandomIt first, RandomIt last, Distance i, Compare comp = Compare()) {
    Distance n = ltl::distance(first, last);
    while (i < n) {
        Distance left = 2 * i + 1;
        Distance right = left + 1;
        Distance max = i;
        if (left < n && comp(*(first + max), *(first + left))) max = left;
        if (right < n && comp(*(first + max), *(first + right))) max = right;
        if (max == i)
            return;
        else {
            std::swap(*(first + i), *(first + max));
            i = max;
        }
    }
}

/*
 * 在范围 [first, last) 中构造最大堆
 */
template <class RandomIt, class Compare = std::less<typename iterator_traits<RandomIt>::value_type>>
void make_heap(RandomIt first, RandomIt last, Compare comp = Compare()) {
    using Distance = typename iterator_traits<RandomIt>::difference_type;
    Distance n = ltl::distance(first, last);
    for (Distance i = (n + 1) / 2 - 1; i >= 0; i--) {
        adjust_heap(first, last, i, comp);
    }
}

/*
 * 插入位于位置 last-1 的元素到范围 [first, last-1) 所定义的最大堆中
 */
template <class RandomIt, class Compare = std::less<typename iterator_traits<RandomIt>::value_type>>
void push_heap(RandomIt first, RandomIt last, Compare comp = Compare()) {
    using Distance = typename iterator_traits<RandomIt>::difference_type;
    using Value = typename iterator_traits<RandomIt>::value_type;
    Distance child = ltl::distance(first, last) - 1;
    Distance parent = (child - 1) / 2;
    Value value = *(first + child);

    while (child > 0 && comp(*(first + parent), value)) {
        *(first + child) = std::move(*(first + parent));
        child = parent;
        parent = (child - 1) / 2;
    }
    *(first + child) = std::move(value);
}

/*
 * 交换在位置 first 的值和在位置 last-1 的值，并令子范围 [first, last-1) 变为堆。
 * 这拥有从范围 [first, last) 所定义的堆移除首个元素的效果。
 */
template <class RandomIt, class Compare = std::less<typename iterator_traits<RandomIt>::value_type>>
void pop_heap(RandomIt first, RandomIt last, Compare comp = Compare()) {
    std::swap(*first, *(last - 1));
    adjust_heap(first, last - 1, 0, comp);
}

/*
 * 转换最大堆 [first, last) 为以升序排序的范围。产生的范围不再拥有堆属性。
 */
template <class RandomIt, class Compare = std::less<typename iterator_traits<RandomIt>::value_type>>
void sort_heap(RandomIt first, RandomIt last, Compare comp = Compare()) {
    using Distance = typename iterator_traits<RandomIt>::difference_type;
    Distance n = ltl::distance(first, last);
    for (Distance i = 0; i < n; i++) {
        std::swap(*first, *(last - 1));
        last--;
        adjust_heap(first, last, 0, comp);
    }
}

}  // namespace ltl

#endif  // LTL_DETAIL_HEAP_H_
