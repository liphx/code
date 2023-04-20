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

## `std::transform`

```cpp
template<class InputIt, class OutputIt, class UnaryOperation>
OutputIt transform(InputIt first1, InputIt last1, OutputIt d_first, UnaryOperation unary_op);

vector<int> data { 1, 2, 3, 4, 5 };
transform(data.begin(), data.end(), ostream_iterator<int>(cout, " "), [](int x) { return x * x; });
// 1 4 9 16 25
```

## `std::generate, std::generate_n`

```cpp
template<class ForwardIt, class Generator>
void generate(ForwardIt first, ForwardIt last, Generator g);

template<class OutputIt, class Size, class Generator>
OutputIt generate_n(OutputIt first, Size count, Generator g);

vector<int> data(10);
generate(data.begin(), data.end(), [n = 0] () mutable { return n++; });
// 0 1 2 3 4 5 6 7 8 9

generate_n(data.begin(), 5, [n = 1] () mutable { return 2 * n++; });
// 2 4 6 8 10 5 6 7 8 9
```

## `std::copy, std::copy_backward`

```cpp
template<class InputIt, class OutputIt>
OutputIt copy(InputIt first, InputIt last, OutputIt d_first);

template<class BidirIt1, class BidirIt2>
BidirIt2 copy_backward(BidirIt1 first, BidirIt1 last, BidirIt2 d_last);

vector<int> data;
copy(istream_iterator<int>(cin), istream_iterator<int>(), back_inserter(data));
copy(data.begin(), data.end(), ostream_iterator<int>(cout, " "));

vector<int> data { 2, 3, 4, 5, 6, 7 };
data.resize(data.size() + 1);
copy_backward(data.begin(), data.begin() + 6, data.end());
data[0] = 1;
copy(data.begin(), data.end(), ostream_iterator<int>(cout, " "));
// 1 2 3 4 5 6 7
```

## `std::is_sorted`

```cpp
template<class ForwardIt>
bool is_sorted(ForwardIt first, ForwardIt last);

template<class ForwardIt, class Compare>
bool is_sorted(ForwardIt first, ForwardIt last, Compare comp);

int arr[] = { 3, 2, 1, 0 };
cout << boolalpha << is_sorted(begin(arr), end(arr)) << endl;       // false
cout << is_sorted(begin(arr), end(arr), greater<int>()) << endl;    // true
```

## `std::sort`

```cpp
template<class RandomIt>
void sort(RandomIt first, RandomIt last);

template<class RandomIt, class Compare>
void sort(RandomIt first, RandomIt last, Compare comp);

int data[5] = { 3,1,2,5,4 };
sort(data, data + 5);
// 1 2 3 4 5
sort(data, data + 5, greater<int>());
// 5 4 3 2 1
```

## `std::binary_search, std::lower_bound, std::upper_bound`

```cpp
template<class ForwardIt, class T>
bool binary_search(ForwardIt first, ForwardIt last, const T& value);

template<class ForwardIt, class T, class Compare>
bool binary_search(ForwardIt first, ForwardIt last, const T& value, Compare comp);

template<class ForwardIt, class T>
ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value);

template<class ForwardIt, class T, class Compare>
ForwardIt lower_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp);

template<class ForwardIt, class T>
ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value);

template<class ForwardIt, class T, class Compare>
ForwardIt upper_bound(ForwardIt first, ForwardIt last, const T& value, Compare comp);

int a[5] = { -2, 5, 7, -3, 4 };
sort(a, a + 5);
// -3 -2 4 5 7

cout << binary_search(a, a + 5, 4) << endl; // 1
cout << binary_search(a, a + 5, 1) << endl; // 0

cout << *lower_bound(a, a + 5, 5) << endl;  // 5
cout << *lower_bound(a, a + 5, 1) << endl;  // 4
cout << *lower_bound(a, a + 4, 8) << endl;  // 7

cout << *upper_bound(a, a + 5, 5) << endl;  // 7
cout << *upper_bound(a, a + 4, 8) << endl;  // 7
```

## `std::next_permutation`

```cpp
template<class BidirIt>
bool next_permutation(BidirIt first, BidirIt last);

template<class BidirIt, class Compare>
bool next_permutation(BidirIt first, BidirIt last, Compare comp);

string str("aba");
sort(str.begin(), str.end());
while(next_permutation(str.begin(), str.end()))
    cout << str << endl;
// aba
// baa
```

## `std::merge`

```cpp
template<class InputIt1, class InputIt2, class OutputIt>
OutputIt merge(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt d_first);

template<class InputIt1, class InputIt2, class OutputIt, class Compare>
OutputIt merge(InputIt1 first1, InputIt1 last1, InputIt2 first2, InputIt2 last2, OutputIt d_first, Compare comp);

array<int, 5> data { 1, 3, 5, 7, 9 };
array<int, 5> data2 { 2, 4, 6, 8, 10 };
array<int, 10> output;
merge(data.begin(), data.end(), data2.begin(), data2.end(), output.begin());
copy(output.begin(), output.end(), ostream_iterator<int>(cout, " "));
```

## `std::reverse`

反转范围内的元素

```cpp
template<class BidirIt>
void reverse(BidirIt first, BidirIt last);

int arr[] = {0, 1, 2, 3, 4};
std::reverse(arr, arr + 5);
std::copy(arr, arr + 5, std::ostream_iterator<int>(std::cout, " "));
// 4 3 2 1 0
```
