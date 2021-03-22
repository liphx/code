#ifndef VECTOR_HPP_
#define VECTOR_HPP_

#include <cstddef>
#include <algorithm>

namespace liph {

template<class T>
class vector {
public:
    vector();
    vector(const vector& other);
    vector<T>& operator=(const vector& other);
    ~vector() { if (size_ > 0) delete []data_; }
    size_t size() { return size_; }
    bool empty() { return size_ == 0; }
    T *data() { return data_; }
    void push_back(const T& x);
    T& operator[](size_t n) { return data_[n]; }
private:
    T *data_;
    size_t size_;
    size_t capacity_;
};

template<class T>
vector<T>::vector():size_(0), capacity_(0) {}

template<class T>
vector<T>::vector(const vector& other)
{
    size_ = other.size_;
    capacity_ = other.capacity_;
    if (capacity_ > 0) {
        data_ = new T[capacity_];
    }
    std::copy(other.data_, other.data_ + size_, data_);
}

template<class T>
vector<T>& vector<T>::operator=(const vector& other)
{
    if (capacity_ >= other.capacity_) {
        std::copy(other.data_, other.data_ + size_, data_);
    } else {
        capacity_ = other.capacity_;
        if (size_ == 0) {
            data_ = new T[capacity_];
        } else {
            data_ = (T *)realloc(data_, capacity_);
            if (data_ == nullptr) {
                throw std::bad_alloc();
            }
        }
        std::copy(other.data_ + size_, other.data_ + other.size_ - size_, data_ + size_);
    }

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
            capacity_ = 1;
        else 
            capacity_ *= 2;
        T *tmp = new T[capacity_];
        std::copy(data_, data_ + size_, tmp);
        if (size_ > 1)
            delete []data_;
        data_ = tmp;
        data_[size_++] = x;
    }
}

} // namespace liph

#endif // VECTOR_HPP_
