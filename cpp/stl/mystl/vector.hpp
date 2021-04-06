#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <cstddef>
#include <algorithm>
#include <stdexcept>
#include <iostream>
#include <iterator>
#include <initializer_list>

namespace liph {

template<class T>
class vector {
public:
    /* constructor */
    vector(size_t capacity = 0);                    // 默认构造函数, 容量初始化为capacity
    vector(std::initializer_list<T> init);          // 构造函数, 初始化列表
    template <class InputIt>
    vector(InputIt first, InputIt last);            // 构造函数, 接受输入迭代器
    vector(const vector<T>& other);                 // 拷贝构造函数
    vector<T>& operator=(const vector<T>& other);   // 复制元素, 修改size, 容量可能不变化

    /* destructor */
    ~vector() { if (capacity_ > 0) delete []data_; }    // 析构, 释放内存

    /* capacity */
    size_t size() { return size_; }             // 返回已存元素大小
    size_t capacity() { return capacity_; }     // 返回已分配内存大小
    bool empty() { return size_ == 0; }         // 容器是否为空
    void reserve (size_t n);                    // 请求改变容量大小, n <= capacity() 无变化

    /* element access */
    T *data() { return data_; }                 // 返回容器中第一个元素的地址
    const T *data() const { return data_; }     // 同上, 返回 const 指针
    T& operator[](size_t n) { return data_[n]; }                // 返回在位置n的元素的引用
    const T& operator[](size_t n) const { return data_[n]; }    // 同上, 返回 const reference
    T& at(size_t n);                            // 同[], 检查越界，抛出 std::out_of_range
    const T& at(size_t n) const;                // 同上, 返回 const reference

    /* modifiers */
    void push_back(const T& x);                 // 在容器末尾添加元素, 若空间不足自动分配内存
    void pop_back();                            // 移除容器的末元素, 若size为0, 无变化

    // serialization
    friend std::ostream& operator<<(std::ostream& o, const vector<T>& v) {
        o << '[';
        for (size_t i = 0; i < v.size_; i++) {
            o << v[i];
            if (i != v.size_ - 1)
                o << ',';
        }
        o << ']';
        return o;
    }

    // iterator;
    class iterator;
    iterator begin() { return iterator(data_); }
    iterator end() { return iterator(data_ + size_); }

    class iterator {
    public:
        typedef std::bidirectional_iterator_tag iterator_category;
        typedef T value_type;
        typedef ptrdiff_t difference_type;
        typedef T* pointer;
        typedef T& reference;

        iterator(T *pos = nullptr): pos_(pos) {}
        T& operator*() const { return *pos_; }
        T* operator->() const { return pos_; }
        iterator& operator++() {
            ++pos_;
            return *this;
        }
        iterator operator++(int) {
            iterator old = *this;
            ++pos_;
            return old;
        }
        iterator& operator--() {
            --pos_;
            return *this;
        }
        iterator operator--(int) {
            iterator old = *this;
            --pos_;
            return old;
        }
        bool operator!=(const iterator& rhs) const {
            return pos_ != rhs.pos_;
        }                
        bool operator==(const iterator& rhs) const {
            return pos_ != rhs.pos_;
        }
    private:
        T *pos_;
    };
    
private:
    T *data_;
    size_t size_;
    size_t capacity_;
};

template<class T>
vector<T>::vector(size_t capacity):size_(0), capacity_(capacity)
{
    if (capacity_ > 0) {
        data_ = new T[capacity_];
    }
}

template<class T>
vector<T>::vector(std::initializer_list<T> init)
{
    capacity_ = size_ = init.size();
    data_ = new T[capacity_];
    std::copy(init.begin(), init.end(), data_);
}

template <class T>
template <class InputIt>
vector<T>::vector(InputIt first, InputIt last)
{
    capacity_ = size_ = std::distance(first, last);
    data_ = new T[capacity_];
    std::copy(first, last, data_);
}

template<class T>
vector<T>::vector(const vector<T>& other)
{
    size_ = other.size_;
    capacity_ = other.capacity_;
    if (capacity_ > 0) {
        data_ = new T[capacity_];
    }
    std::copy(other.data_, other.data_ + other.size_, data_);
}

template<class T>
vector<T>& vector<T>::operator=(const vector<T>& other)
{
    if (capacity_ < other.capacity_) {
        reserve(other.capacity_);
    }
    std::copy(other.data_, other.data_ + other.size_, data_);
    size_ = other.size_;
    return *this;
}

template<class T>
void vector<T>::push_back(const T& x)
{
    if (size_ < capacity_) {
        data_[size_++] = x;
    } else {
        if (capacity_ == 0)
            reserve(1);
        else 
            reserve(capacity_ * 2);
        data_[size_++] = x;
    }
}

template<class T>
void vector<T>::pop_back()
{
    if (size_ == 0)
        return;
    data_[--size_].~T(); // 是否会调用两次
}

template<class T>
T& vector<T>::at(size_t n)
{
    if (n >= size_)
        throw std::out_of_range("argument >= this->size()");
    return data_[n];
}

template<class T>
const T& vector<T>::at(size_t n) const
{
    if (n >= size_)
        throw std::out_of_range("argument >= this->size()");
    return data_[n];
}  

template<class T>//capac
void vector<T>::reserve(size_t n)
{
    if (n <= capacity_)
        return;
    if (capacity_ == 0) {
        data_ = new T[n];
        capacity_ = n;
    } else {
        T *tmp = new T[n];
        if (tmp == nullptr) 
            throw std::bad_alloc();
        std::copy(data_, data_ + size_, tmp);
        delete []data_;
        data_ = tmp;
        
        capacity_ = n;
    }
}

} // namespace liph

#endif // VECTOR_HPP_
