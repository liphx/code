#include <cassert>

int main() {
    // 不加括号, 逗号将当成宏参数的分割符
    assert(("test1", true));
    assert(("test2", false));
}
