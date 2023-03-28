#ifndef LIPH_SORT_H_
#define LIPH_SORT_H_

#include <functional>  // less
#include <iterator>    // iterator_traits
#include <vector>      // for bucket_sort

namespace liph {

template <class RandomIt, class Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void bubble_sort(RandomIt first, RandomIt last, Compare comp = Compare()) {
    using Distance = typename std::iterator_traits<RandomIt>::difference_type;
    Distance n = std::distance(first, last);
    for (Distance i = 0; i < n - 1; i++) {
        for (Distance j = 0; j < n - 1; j++) {
            if (!comp(*(first + j), *(first + j + 1))) {
                std::swap(*(first + j), *(first + j + 1));
            }
        }
    }
}

template <class RandomIt, class Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void selection_sort(RandomIt first, RandomIt last, Compare comp = Compare()) {
    using Distance = typename std::iterator_traits<RandomIt>::difference_type;
    Distance n = std::distance(first, last);
    for (Distance i = 0; i < n - 1; i++) {
        Distance min_index = i;
        for (Distance j = i + 1; j < n; j++) {
            if (comp(*(first + j), *(first + min_index))) {
                min_index = j;
            }
        }
        if (min_index != i) {
            std::swap(*(first + i), *(first + min_index));
        }
    }
}

template <class RandomIt, class Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
RandomIt partition(RandomIt first, RandomIt last, const Compare& comp = Compare()) {
    using value_type = typename std::iterator_traits<RandomIt>::value_type;
    value_type pivotkey = *first;
    while (first != last) {
        while (first != last && !comp(*(last - 1), pivotkey)) last--;
        if (first != last) *first++ = *(last - 1);

        while (first != last && comp(*first, pivotkey)) first++;
        if (first != last) *(last - 1) = *first;
    }
    *first = pivotkey;
    return first + 1;
}

template <class RandomIt, class Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void quick_sort(RandomIt first, RandomIt last, Compare comp = Compare()) {
    if (std::distance(first, last) > 1) {
        RandomIt it = liph::partition(first, last, comp);
        liph::quick_sort(first, it, comp);
        liph::quick_sort(it, last, comp);
    }
}

/*
 * 将有序的[first, mid) 和 [mid, last) 合并为有序的 [first, last)
 */
template <class RandomIt, class Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void merge(RandomIt first, RandomIt mid, RandomIt last, Compare comp = Compare()) {
    using value_type = typename std::iterator_traits<RandomIt>::value_type;
    using Distance = typename std::iterator_traits<RandomIt>::difference_type;
    Distance n = std::distance(first, last);

    value_type *tmp = new value_type[n];
    RandomIt i = first, j = mid;
    value_type *k = tmp;
    while (i < mid && j < last) {
        if (comp(*i, *j))
            *k++ = *i++;
        else
            *k++ = *j++;
    }

    while (i < mid) *k++ = *i++;
    while (j < last) *k++ = *j++;

    std::copy(tmp, tmp + n, first);
    delete[] tmp;
}

template <class RandomIt, class Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void merge_sort(RandomIt first, RandomIt last, Compare comp = Compare()) {
    using Distance = typename std::iterator_traits<RandomIt>::difference_type;
    Distance n = std::distance(first, last);
    if (n <= 1) return;

    RandomIt mid = first + n / 2;
    liph::merge_sort(first, mid, comp);
    liph::merge_sort(mid, last, comp);
    liph::merge(first, mid, last, comp);
}

template <class RandomIt, class Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void insert_sort(RandomIt first, RandomIt last, Compare comp = Compare()) {
    using Distance = typename std::iterator_traits<RandomIt>::difference_type;
    using value_type = typename std::iterator_traits<RandomIt>::value_type;
    Distance n = std::distance(first, last);
    // 从第二个元素起，小于前一个元素才需要插入
    for (Distance i = 1; i < n; i++) {
        if (comp(*(first + i), *(first + i - 1))) {
            value_type x = *(first + i);
            Distance j;
            for (j = i - 1; j >= 0 && comp(x, *(first + j)); j--) *(first + j + 1) = *(first + j);
            *(first + j + 1) = x;
        }
    }
}

/*
 * 增量为 dk 的插入排序
 * dk = 1 => insert_sort
 */
template <class RandomIt, class Distance,
        class Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void shell_insert(RandomIt first, RandomIt last, Distance dk, Compare comp = Compare()) {
    using value_type = typename std::iterator_traits<RandomIt>::value_type;
    Distance n = std::distance(first, last);
    for (Distance i = 0; i < dk; i++) {
        for (Distance j = i + dk; j < n; j += dk) {
            if (comp(*(first + j), *(first + j - dk))) {
                value_type x = *(first + j);
                Distance k;
                for (k = j - dk; k >= 0 && comp(x, *(first + k)); k -= dk) {
                    *(first + k + dk) = *(first + k);
                }
                *(first + k + dk) = x;
            }
        }
    }
}

template <class RandomIt, class Compare = std::less<typename std::iterator_traits<RandomIt>::value_type>>
void shell_sort(RandomIt first, RandomIt last, Compare comp = Compare()) {
    using Distance = typename std::iterator_traits<RandomIt>::difference_type;
    Distance n = std::distance(first, last);
    // dk 为增量，dk/2 上取整
    for (Distance dk = (n + 1) / 2; dk > 1; dk = (dk + 1) / 2) {
        shell_insert(first, last, dk, comp);
    }
    // same as insert_sort(first, last, comp);
    shell_insert(first, last, 1, comp);
}

/*
 * 计数排序
 * 对于 A[0, n)中的每个元素 i, 0 <= i < max
 * n > 0, k > 0
 */
void counting_sort(int A[], int n, int max) {
    // int count[max] = {0};
    int *count = new int[max]{0};
    for (int i = 0; i < n; i++) {
        count[A[i]]++;
    }

    int index = 0;
    for (int i = 0; i < max; i++) {
        for (int j = 0; j < count[i]; j++) A[index++] = i;
    }
    delete[] count;
}

/*
 * 基数排序
 * A[0, n)中的每个元素不超过 d 位, 例如 d = 3 时，最大元素为999
 * n > 0, d > 0
 */
void radix_sort(int A[], int n, int d) {
    int *tmp = new int[n];
    int count[10] = {0};
    int factor = 1;
    int flag = 0;  // 标记对哪个数组排序
    for (int i = 0; i < d; i++) {
        // 从最低位开始排序
        factor *= 10;
        int *arr1 = (flag == 0 ? A : tmp);
        int *arr2 = (flag == 0 ? tmp : A);
        // clear count
        for (int j = 0; j < 10; j++) {
            count[j] = 0;
        }

        for (int j = 0; j < n; j++) {
            int base = (arr1[j] % factor) / (factor / 10);
            count[base]++;
        }

        // recount
        for (int j = 9; j > 0; j--) count[j] = count[j - 1];
        count[0] = 0;
        for (int j = 1; j < 10; j++) count[j] += count[j - 1];

        for (int j = 0; j < n; j++) {
            int base = (arr1[j] % factor) / (factor / 10);
            arr2[count[base]++] = arr1[j];
        }

        flag = 1 - flag;
    }

    if (flag) {
        for (int i = 0; i < n; i++) {
            A[i] = tmp[i];
        }
    }

    delete[] tmp;
}

/*
 * 桶排序
 * 假定A[0, n) 均匀分布在[min, max) 区间，桶的个数为 m
 * n > 0, min < max, m > 0
 */
void bucket_sort(int A[], int n, int min, int max, int m) {
    std::vector<int> *tmp = new std::vector<int>[m];
    int distance = max - min;
    int count = (distance - 1) / m + 1;  // ceil
    for (int i = 0; i < n; i++) {
        tmp[(A[i] - min) / count].emplace_back(A[i]);
    }

    int index = 0;
    for (int i = 0; i < m; i++) {
        liph::insert_sort(tmp[i].begin(), tmp[i].end());
        for (size_t j = 0; j < tmp[i].size(); j++) {
            A[index++] = tmp[i][j];
        }
    }

    delete[] tmp;
}

}  // namespace liph

#endif  // LIPH_SORT_H_
