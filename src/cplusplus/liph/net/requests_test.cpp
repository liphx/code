#include "liph/net/requests.h"

#include "gtest/gtest.h"

TEST(requests, requests) { auto res = liph::requests::get("https://liph.ink"); }
