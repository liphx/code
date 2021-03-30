#include "json.hpp"
#include <gtest/gtest.h>

TEST(json, constructor)
{
    liph::json j;
    j["pi"] = 3.14;
    j["name"] = "liph";
    j["bool"] = true;
    j["null"] = nullptr;
    j["object"]["a"] = "hello";
    j["object"]["b"] = "world";

    std::string str = R"({"pi": 3.14, "name": "liph", "bool": true, "null": null, "object": {"a": "hello", "b": "world"}})";
    liph::json j2 = liph::json::parse(str);

    ASSERT_TRUE(j == j2);
}
