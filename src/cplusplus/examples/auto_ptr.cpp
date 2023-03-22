#include <iostream>
using namespace std;

/*
 * enable asan
 * g++ -g -fsanitize=address -fsanitize-recover=address -lasan
 */

template <class T>
class auto_ptr {
public:
    auto_ptr(T *p = nullptr) { element = p; }

    auto_ptr(auto_ptr& other) {  // 需要修改 other
        element = other.element;
        other.element = nullptr;
    }

    T *get() const { return element; }

    T& operator*() const { return *get(); }

    T *operator->() const { return get(); }

    ~auto_ptr() {
        if (element) delete element;
    }

private:
    T *element;
};

int main() {
    auto_ptr<int> pa(new int(100));
    cout << *pa << endl;
    *pa = 200;
    cout << *pa << endl;

    auto_ptr<int> pb(pa);
    // cout << *pa << endl;
    cout << *pb << endl;
}
