#include "liph/net/requests.h"

#include "gtest/gtest.h"

TEST(requests, requests) { liph::requests::get("https://liph.ink"); }
