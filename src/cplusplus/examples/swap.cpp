#include <cstdio>
#include <utility>

struct A {
    void swap(A&) { puts("A::swap"); }
};

template <class T>
struct B {
    void swap(B&) { puts("B<>::swap"); }
};

// a function definition added to namespace std:
// undefined behavior
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
        std::swap(a, b);  // A::swap
    }

    {
        B<int> a, b;
        std::swap(a, b);  // B<>::swap
    }
}
