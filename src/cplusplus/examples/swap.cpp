#include "liph/liph.h"

struct A {
    void swap(A&) { liph::print("A::swap"); }
};

template <class T>
struct B {
    void swap(B&) { liph::print("B<>::swap"); }
};

namespace std {
template <>
void swap<A>(A& a, A& b) noexcept {
    a.swap(b);
}

template <class T>
void swap(B<T>& a, B<T>& b) noexcept {
    a.swap(b);
}
}  // namespace std

int main() {
    {
        int a, b;
        std::swap(a, b);
    }

    {
        A a, b;
        std::swap(a, b);
    }

    {
        B<int> a, b;
        std::swap(a, b);
    }
}
