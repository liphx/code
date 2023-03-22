template <class T>
void print(const vector<T>& vc) {
    if (vc.empty()) return;
    /* vector<int>::const_iterator = vc.cbegin(); */
    // 必须要有 typename
    typename vector<int>::const_iterator it = vc.begin();
    cout << *it << endl;

    // or
    auto it2 = vc.begin();
    cout << *it2 << endl;

    typedef typename vector<int>::const_iterator iter;
    iter it3 = vc.begin();

    using iter2 = typename vector<int>::const_iterator;
    iter2 it4 = vc.begin();
}

template <class T>
/* void add(vector<T>& vc, vector<T>::const_iterator it) */
// 必须要有 typename
void add(vector<T>& vc, typename vector<T>::const_iterator it) {
    vc.emplace_back(*it);
}

template <class T>
class A {
    struct Nested {};
};

template <class T>
/* class B: public typename A<T>::Nested {}; */
// 不能有 typename
class B : public A<T>::Nested {
public:
    /* B(): typename A<T>::Nested() {} */
    // 不能有 typename
    B() : A<T>::Nested() {
        /* A<T>::Nested tmp; */
        // 必须要有 typename
        typename A<T>::Nested tmp;
    }
};
