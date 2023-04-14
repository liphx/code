#include "liph/string.h"

#include "gtest/gtest.h"
#include "liph/print.h"

TEST(string, string) {
    std::string str = "\f\r  hello  \n\t ";
    D(str);
    D(liph::ltrim(str));
    D(liph::rtrim(str));
    D(liph::trim(str));

    str = "world";
    D(str);
    D(liph::startswith(str, "wor"));
    D(liph::startswith(str, "wr"));
    D(liph::endswith(str, "ld"));
    D(liph::endswith(str, "l"));

    T(str = " 1   2   1 34 "; std::vector<std::string> tokens; liph::split(tokens, str); P(tokens););

    D(liph::from_string<int>("123"));
    D(liph::to_string(std::vector<int>{1, 2, 3}));
    D((std::pair<std::string, int>("hello", 100)));
}