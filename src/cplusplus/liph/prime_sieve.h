#ifndef LIPH_PRIME_SIEVE_H_
#define LIPH_PRIME_SIEVE_H_

#include <cassert>
#include <cstring>
#include <string>

namespace liph {

class prime_sieve {
public:
    static const int max_size = 8 * 1024 * 1024;  // 8388608, 1MB
    prime_sieve(int max = 0) : max_(std::min(max, max_size)) { init(); }

    ~prime_sieve() { delete[] bits_; }

    bool is_prime(int n) {
        if (n < 2) return false;
        if (n <= max_) return get_bit(n);
        if (n <= max_size) {
            max_ = n;
            init();
            return is_prime(n);
        }
        return is_prime_loop(n);
    }

private:
    void init() {
        if (bits_) delete[] bits_;
        if (max_ <= 0) return;
        bits_ = new char[max_ / 8 + 1];
        memset(bits_, -1, max_ / 8 + 1);
        for (int i = 2; i <= max_; i++) {
            if (get_bit(i)) {
                for (int j = i + i; j <= max_; j += i) {
                    set_bit_false(j);
                }
            }
        }
    }

    bool is_prime_loop(int n) const {
        assert(n > max_size);
        for (int i = 2; i * i <= n; i++) {
            if (n % i == 0) return false;
        }
        return true;
    }

    void set_bit_false(int n) {
        assert(n <= max_);
        n -= 2;
        char& ch = bits_[n / 8];
        ch &= ~(1 << (7 - n % 8));
    }

    bool get_bit(int n) const {
        assert(n <= max_);
        n -= 2;
        char ch = bits_[n / 8];
        return ch & (1 << (7 - n % 8));
    }

    std::string debug_string() const {
        std::string str;
        int i = 1;
        while (i <= max_) {
            str += get_bit(i++) ? '1' : '0';
        }
        return str;
    }

private:
    int max_{0};
    char *bits_{nullptr};
};

}  // namespace liph

#endif  // LIPH_PRIME_SIEVE_H_
