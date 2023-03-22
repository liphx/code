#include "json.h"

#include "gtest/gtest.h"

namespace liph {
namespace test {

TEST(json, size) { EXPECT_EQ(sizeof(json), 16u); }

TEST(json, constructor) {
    EXPECT_EQ(json().to_string(), "null");
    EXPECT_EQ(json(true).to_string(), "true");
    EXPECT_EQ(json(false).to_string(), "false");
    EXPECT_EQ(json((double)42).to_string(), "42");
    EXPECT_EQ(json(3.14).to_string(), "3.14");
    EXPECT_EQ(json("hello, world").to_string(), "\"hello, world\"");
}

TEST(json, parse) {
    EXPECT_EQ(json::parse("null"), json());
    EXPECT_EQ(json::parse("  true  "), json(true));
    EXPECT_EQ(json::parse("\"hello\""), json("hello"));
    EXPECT_EQ(json::parse("42"), json((double)42));

    std::string str =
            R"({"encoding": "UTF-8", "plug-ins": ["python", "c++", "ruby"], "indent": {"length": 3, "use_space": true}})";
    json j1 = json::parse(str);

    std::vector<json> arr = {json("python"), json("c++"), json("ruby")};
    std::unordered_map<std::string, json> sub = {{"length", json((double)3)}, {"use_space", json(true)}};

    std::unordered_map<std::string, json> obj = {
            {"encoding", json("UTF-8")}, {"plug-ins", json(arr)}, {"indent", json(sub)}};

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
