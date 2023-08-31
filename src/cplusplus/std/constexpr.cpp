#include <string>

static const int a = 0;  // static is not necessary
const int b = 0;         // const means static unless declared extern

constexpr static const int c = 0;  // static and const are not necessary
constexpr int d = 0;               // ok

// constexpr std::string str = "hello, world";
// error
constexpr char str2[] = "hello, world";  // ok, const char[13]
constexpr auto str3 = "hello, world";    // ok, const char *const

int main() {}
