#include <iostream>
#include <iterator>
#include <vector>
using namespace std;

template <class T>
void foo(const vector<T>& vc) {
    // vector<T>::const_iterator it = vc.cbegin();
    typename vector<T>::const_iterator it = vc.cbegin();

    auto it2 = vc.begin();

    typedef typename vector<T>::const_iterator Iter;
    Iter it3 = vc.begin();

    using It = typename vector<T>::const_iterator;
    It it4 = vc.begin();
}

template <class T>
// void add(vector<T>& vc, vector<T>::const_iterator it)
void add(vector<T>& vc, typename vector<T>::const_iterator it) {
    vc.emplace_back(*it);
}

template <class T>
struct A {
    struct Nested {};
};

template <class T>
// class B: typename A<T>::Nested {};
struct B : A<T>::Nested {
    // B(): typename A<T>::Nested() {}
    B() : A<T>::Nested() {
        // A<T>::Nested tmp;
        typename A<T>::Nested tmp;
    }
};

int main() {
    vector v{1, 2, 3, 4};
    vector v2{0};
    foo(v);
    add(v2, v.cbegin());
}
