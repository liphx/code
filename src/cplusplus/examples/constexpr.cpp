#include <iostream>
#include <string>

static const int a = 0;  // static is not necessary
const int b = 0;         // const means static unless declared extern

constexpr static const int c = 0;  // static and const are not necessary
constexpr int d = 0;               // ok

// constexpr std::string str = "hello, world";
// error
constexpr char str2[] = "hello, world";  // ok, const char[13]
constexpr auto str3 = "hello, world";    // ok, const char *const

constexpr int foo(int x) { return x; }
consteval int bar(int x) { return x; }

void func() {
    int n;
    std::cin >> n;
    char arr[n];
    // constexpr size_t size = sizeof(arr);
    // std::cout << size << std::endl;
}

int main() {
    constexpr int x = foo(42);
    constexpr int y = foo(x);

    {
        int z = 100;
        // error
        // constexpr int z2 = foo(z);
        int z2 = foo(z);
    }

    {
        const int z = 100;
        constexpr int z2 = foo(z);
    }

    {
        int z = 100;
        // error
        // bar(z);
    }

    {
        const int z = 100;
        bar(z);
    }
}
