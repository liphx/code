# 异常

```cpp
#include <exception>

typedef void (*terminate_handler)();
std::terminate_handler get_terminate() noexcept;
std::terminate_handler set_terminate(std::terminate_handler f) noexcept;
[[noreturn]] void terminate() noexcept;
```
