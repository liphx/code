# constexpr

```cpp
static const int i = 0; // static is not necessary
const int i = 0;        // const means static unless declared extern

constexpr static const int i = 0;   // static and const are not necessary
constexpr int i = 0;                // ok

constexpr std::string str = "hello, world"; // error
constexpr char str[] = "hello, world";      // ok, const char[13]
constexpr auto str = "hello, world";        // ok, const char *const
```
