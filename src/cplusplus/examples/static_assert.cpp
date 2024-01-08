static_assert(0 < 1, "impossible");
static_assert(0 < 1);  // default message

int main() {
    static_assert(0 < 1, "impossible");
    // static_assert(2 == 3, "static_assert");
}
