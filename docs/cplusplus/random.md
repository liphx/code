# C++中的随机数生成

## 来自C库的函数

```c
#include <stdlib.h>

#define RAND_MAX			// implementation defined, >= 32767
int rand();					// return [0, RAND_MAX]
void srand(unsigned seed);	// seeds the pseudo-random number generator used by rand()
```

在调用任何`srand`前调用`rand`，相当于`srand(1)`。

C++中的`rand, srand`同C语言，定义在`namespace std`中。

```cpp
#include <cstdlib>

#define RAND_MAX

namespace std {
	int rand();
	void srand(unsigned int seed);
}
```

`rand`采用的算法是实现定义的，因此尽管以同一个seed值播种，不同系统下`rand()`得到的结果也可能不同。

`rand`被视为低质量随机数生成器，尽量避免在严肃的随机数生成场合使用。

`32767 == INT16_MAX`，该值与C++标准规定的int最小位宽一致，实现中，一般`RAND_MAX == INT_MAX`。

`rand()`不保证可重入或者线程安全，可重入的版本是POSIX中的[rand_r](https://linux.die.net/man/3/rand_r)。

## C++库`<random>`

```cpp
#include <random>

namespace std {

class random_device {
public:
    using result_type = unsigned int;
    static constexpr result_type min() { return numeric_limits<result_type>::min(); }
    static constexpr result_type max() { return numeric_limits<result_type>::max(); }

    random_device() : random_device(/*implementation-defined*/) {}
    explicit random_device(const string& token);
    random_device(const random_device&) = delete;
    void operator=(const random_device&) = delete;

    result_type operator()(); // generating
    double entropy() const noexcept;
};

using default_random_engine = /*implementation-defined*/; // predefined random number generator

template <class IntType = int>
class uniform_int_distribution {
public:
    using result_type = IntType;
    using param_type = /*the type of the parameter set*/;

    uniform_int_distribution() : uniform_int_distribution(0) {}
    explicit uniform_int_distribution(IntType a, IntType b = numeric_limits<IntType>::max());
    explicit uniform_int_distribution(const param_type& parm);

    void reset();

    // generating
    template <class URBG>
    result_type operator()(URBG& g);
    template <class URBG>
    result_type operator()(URBG& g, const param_type& parm);

    result_type a() const;
    result_type b() const;
    param_type param() const;
    void param(const param_type& parm);
    result_type min() const;
    result_type max() const;
};

}  // namespace std
```

`std::random_device`通常仅用于播种伪随机数生成器。

```cpp
std::random_device rd;
std::default_random_engine re(rd());
std::uniform_int_distribution<int> distrib(0, 9);
auto rand = std::bind(distrib, re);
for (int i = 0; i < 10; i++) {
	std::cout << rand() << " ";
}
// Possible output:
// 0 9 3 8 1 7 8 5 8 4
```

## 随机排序

```cpp
template <class T>
inline void random_sort(T arr[], int n, std::function<int()> rand = std::rand) {
    for (int i = 0; i < n; i++) {
        int j = rand() % (n - i);
        std::swap(arr[j], arr[n - i - 1]);
    }
}

template <class RandomIt>
inline void random_sort(RandomIt first, RandomIt last, std::function<int()> rand = std::rand) {
    return random_sort(first, std::distance(first, last), rand);
}
```
