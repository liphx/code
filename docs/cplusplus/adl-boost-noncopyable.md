# C++ ADL 与 boost::noncopyable

C++ [ADL](https://zh.cppreference.com/w/cpp/language/adl) 即实参依赖查找（argument-dependent lookup）, 允许调用其他命名空间中的函数。

最常见的例子为 `operator<<`

```cpp
#include <iostream>

int main() {
    operator<<(std::cout, "hello\n");
    // 等同 std::cout << "hello\n";
}
```

`operator<<` 并不存在于全局命名空间, 但左实参 `std::cout` 在命名空间`std`中, 因此找到了
`std::operator<<(std::ostream&, const char*)`

同理, 下面的代码也成立

```cpp
namespace A {
    class B {};
    void func(const B&) {}
}

int main() {
    A::B b;
    func(b);
}
```

如果此时定义全局函数 `void func(const A::B&) {}` , 则编译器报错, 对`func`的调用是`ambiguous`

ADL 在继承中也适用, 例如

```cpp
namespace A {
    class B {};
    void func(const B&) {}
}

class C: public A::B {};
// 如果是private继承则不能转换成 A::B

int main() {
    C c;
    func(c);
}
```

有些时候我们并不希望派生类查找基类所在命名空间的函数, 来看看 `boost::noncopyable` 的做法

```cpp
namespace boost {

namespace noncopyable_ { // protection from unintended ADL

struct base_token {};

class noncopyable: base_token {
#if __cplusplus < 201103L
protected:
    noncopyable() {}
    ~noncopyable() {}
private:
    noncopyable(const noncopyable&);
    noncopyable& operator=(const noncopyable&);
#else
protected:
    constexpr noncopyable() = default;
    ~noncopyable() = default;
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
#endif
};

} // namespace noncopyable_

typedef noncopyable_::noncopyable noncopyable;

} // namespace boost
```

`boost::noncopyable` 的作用是在C++中提供一种方案, 使得继承自该类的对象不能被拷贝(noncopyable, 顾名思义).
为了避免ADL, `noncopyable` 并非直接定义在 `boost` 中的一个类, 而是独立定义在`noncopyable_`, 该命名空间内没有其他函数.
