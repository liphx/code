#include "liph/print.h"

#include "gtest/gtest.h"

using namespace std::literals::string_literals;

namespace liph {

TEST(print, print) {
    println();
    print(true);
    print(false);
    print(42);
    print(42, "hello", 3.14, false);
    print(true, 42, "hello", "wrold");
    print(std::pair{42, "hello"s});
    print(std::vector{1, 2, 3, 4});
    print(std::vector{std::vector{1, 2}, std::vector{3, 4}});
    print(std::list{1, 2, 3, 4});
    print(std::map<int, int>{{1, 2}, {3, 4}});
    print(std::unordered_map<int, int>{{1, 2}, {3, 4}});
    print(std::set{1, 2, 3, 4});
    println();

    int answer = 42;
    PRINT("answer:", answer);
    TRACE(++answer);
    DEBUG(answer);
}

}  // namespace liph
