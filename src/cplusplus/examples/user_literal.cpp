constexpr int operator""_k(unsigned long long n) { return n * 1000; }

int main() {
    static_assert(1_k == 1000);
    static_assert(2_k - 121 == 1_k + 879);
}
