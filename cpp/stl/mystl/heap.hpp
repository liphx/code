#ifndef HEAP_HPP_
#define HEAP_HPP_

namespace liph {

/*
 * 检查范围 [first, last) 并寻找始于 first 且为最大堆的最大范围
 * 使用 operator< 比较元素
 * 返回始于 first 且为最大堆的最大范围上界。即满足范围 [first, it) 是最大堆的最末迭代器 it 
 */
template<class RandomIt>
RandomIt is_heap_until(RandomIt first, RandomIt last)
{
    using Distance = typename RandomIt::difference_type;
    Distance n = last - first; // 随机访问迭代器
    Distance parent = 0;
    for (Distance child = 1; child < n; child++) {
        if (*(first + parent) < *(first + child))
            return first + child;
        if ((child & 1) == 0)
            parent++;
    }
    return last;
}

/*
 * 检查范围 [first, last) 中的元素是否为最大堆
 * 使用 operator< 比较元素
 * 若范围为最大堆则为 true, 否则为 false
 */
template<class RandomIt>
inline bool is_heap(RandomIt first, RandomIt last)
{
    return liph::is_heap_until(first, last) == last;
}
    
} // namespace liph

#endif // HEAP_HPP_
