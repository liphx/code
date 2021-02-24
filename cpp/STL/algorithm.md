# C++ 算法库

`#include <algorithm>`

## `std::for_each`

```cpp
template<class InputIt, class UnaryFunction>
UnaryFunction for_each(InputIt first, InputIt last, UnaryFunction f);

std::vector<int> data { 1, 2, 3 };
std::for_each(data.begin(), data.end(), [](const int& x){ std::cout << x << " "; }); //=>1 2 3 
```

## `std::all_of, std::any_of, std::none_of`

```cpp
template<class InputIt, class UnaryPredicate>
bool all_of(InputIt first, InputIt last, UnaryPredicate p);

template<class InputIt, class UnaryPredicate>
bool any_of(InputIt first, InputIt last, UnaryPredicate p);

template<class InputIt, class UnaryPredicate>
bool none_of(InputIt first, InputIt last, UnaryPredicate p);

std::vector<int> data { 1, 2, 3 };
std::cout << std::all_of(data.begin(), data.end(), [](const int& x) { return x > 0; }); //=>1
std::cout << std::any_of(data.begin(), data.end(), [](const int& x) { return x > 0; }); //=>1
std::cout << std::none_of(data.begin(), data.end(), [](const int& x) { return x > 0; }); //=>0
```

## `std::count, std::count_if`

```cpp
template<class InputIt, class T>
typename iterator_traits<InputIt>::difference_type count(InputIt first, InputIt last, const T &value);

template<class InputIt, class UnaryPredicate>
typename iterator_traits<InputIt>::difference_type count_if(InputIt first, InputIt last, UnaryPredicate p);

std::vector<int> data { 1, 1, 2 };
std::cout << std::count(data.begin(), data.end(), 1); //=>2
std::cout << std::count_if(data.begin(), data.end(), [](const int& x) { return x %2 == 0; }); //=>1
```
