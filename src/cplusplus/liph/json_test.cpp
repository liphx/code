#include "liph/json.h"

#include "gtest/gtest.h"

namespace liph {
namespace test {

TEST(json, size) { EXPECT_EQ(sizeof(json), 16u); }

TEST(json, constructor) {
    EXPECT_EQ(json(), json::null);
    EXPECT_EQ(json(true).bool_ref(), true);
    EXPECT_EQ(json(false).bool_ref(), false);
    EXPECT_EQ(json(42).i64_ref(), 42);
    EXPECT_EQ(json(3.14).double_ref(), 3.14);
    EXPECT_EQ(json("hello, world").string_ref(), "hello, world");
}

TEST(json, parse) {
    EXPECT_EQ(json::parse("null"), json::null);
    EXPECT_EQ(json::parse("  true  "), true);
    EXPECT_EQ(json::parse("\"hello\""), "hello");
    EXPECT_EQ(json::parse("42"), 42);

    std::string str =
            R"({"encoding": "UTF-8", "plug-ins": ["python", "c++", "ruby"], "indent": {"length": 3, "use_space": true}})";
    json j1 = json::parse(str);

    std::vector<json> arr = {"python", "c++", "ruby"};
    std::unordered_map<std::string, json> sub = {{"length", 3}, {"use_space", true}};
    std::unordered_map<std::string, json> obj = {{"encoding", "UTF-8"}, {"plug-ins", arr}, {"indent", sub}};
    json j2(obj);
    EXPECT_EQ(j1, j2);
}

TEST(json, exception) {
    std::string str1 = "{abc:123}";
    EXPECT_THROW(json::parse(str1), std::exception);

    std::string str2 = R"(
    {
      "pi": 3.141,
      "happy": true,
      "name": "Niels",
      "nothing": null,
      "answer": {
        "everything": 42
      },
      "list": [1, 0, 2],
      "object": {
        "currency": "USD",
        "value": 42.99
      }
    }
    )";
    EXPECT_NO_THROW(json::parse(str2));
}

}  // namespace test
}  // namespace liph
