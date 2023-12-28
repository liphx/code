#include <concepts>
#include <iostream>
#include <list>
#include <thread>
#include <vector>
using namespace std::literals;

// constrained template
template <std::copyable T>
class Vector {
private:
    T *elem;
    int sz;
};

template <typename S>
concept Sequence = requires(S a) {
    typename std::ranges::range_value_t<S>;
    typename std::ranges::iterator_t<S>;
    { a.begin() } -> std::same_as<std::ranges::iterator_t<S>>;
    { a.end() } -> std::same_as<std::ranges::iterator_t<S>>;
    requires std::input_iterator<std::ranges::iterator_t<S>>;
    requires std::same_as<std::ranges::range_value_t<S>, std::iter_value_t<S>>;
};

// or this
// template <typename S>
// concept Sequence = std::ranges::input_range<S>;

template <typename T, typename U = T>
concept Number = requires(T x, U y) {
    x + y;
    x - y;
    x *y;
    x / y;
    x += y;
    x -= y;
    x *= y;
    x /= y;
    x = x;  // copy
    x = 0;
};

template <typename T, typename U = T>
concept Arithmetic = Number<T, U> && Number<U, T>;

template <Sequence Seq, class Num>
    requires Arithmetic<std::ranges::range_value_t<Seq>, Num>
Num sum(Seq s, Num v) {
    for (const auto& x : s) v += x;
    return v;
}

template <class Iter>
void advance(Iter p, int n) {
    std::cout << "forward iterator" << std::endl;
    while (n--) ++p;
}

template <class Iter>
    requires requires(Iter p, int i) {
        p[i];
        p + i;
    }
void advance(Iter p, int n) {
    std::cout << "random iterator" << std::endl;
    p += n;
}

template <class B>
concept Boolean = requires(B x, B y) {
    { x = true };
    { x = false };
    { x = (x == y) };
    { x = (x != y) };
    { x = !x };
    { x = (x = y) };
};

// std::equality_comparable
template <typename T, typename T2 = T>
concept EqualityComparable = requires(T a, T2 b) {
    { a == b } -> Boolean;  // compare a T to a T2 with ==
    { a != b } -> Boolean;  // compare a T to a T2 with !=
    { b == a } -> Boolean;  // compare a T2 to a T with ==
    { b != a } -> Boolean;  // compare a T2 to a T with !=
};
static_assert(EqualityComparable<int>);
static_assert(!EqualityComparable<std::thread>);
static_assert(EqualityComparable<int, double>);
static_assert(!EqualityComparable<int, std::string>);

auto twice(Arithmetic auto x) { return x + x; }

int main() {
    Vector<int> v;

    // Vector<int&> v;
    // int& is not copyable

    // Vector<std::thread> threads;
    // std::thread is not copyable

    std::cout << sum(std::vector{1, 2, 3}, 0) << std::endl;  // 6

    {
        std::list list{1, 2, 3};
        auto it = list.begin();
        advance(it, 2);
    }
    {
        std::vector v{1, 2, 3};
        auto it = v.begin();
        advance(it, 2);
    }

    std::cout << twice(1) << std::endl;  // 2
    // twice(std::string("hello"));      // error

    Arithmetic auto i = 0;
    // Arithmetic auto s = "hello"s;
}
