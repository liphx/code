#include <cassert>

int main() {
    // add an additional message to assert errors
    // use `()` to avoid `,` be interpreted as macro argument separators
    assert(("test1", true));
    assert(("test2", false));
}
