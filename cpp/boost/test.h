#ifndef _TEST_H_
#define _TEST_H_

#include <vector>
std::vector<void (*)()> _test_func;

#define TEST(case)              \
    void _test_##case();        \
    void *_test_##case##_unused = [](void (*f)()) { _test_func.emplace_back(f); return nullptr; }(_test_##case); \
    void _test_##case()

static void RUN_ALL_TEST()
{
    for (auto f: _test_func) f();
}

#endif
