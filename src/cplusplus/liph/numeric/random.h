#ifndef LIPH_NUMERIC_RANDOM_H_
#define LIPH_NUMERIC_RANDOM_H_

#include <cstdint>
#include <random>

namespace liph {

struct random {
    /// generate random n bytes
    static bool next_bytes(void *ptr, size_t n);

    /// generate random int32_t
    static int32_t next_int32() { return next<int32_t>(); }

    /// generate random uint32_t
    static uint32_t next_uint32() { return next<uint32_t>(); }

    /// generate random int64_t
    static int64_t next_int64() { return next<int64_t>(); }

    /// generate random uint64_t
    static uint64_t next_uint64() { return next<uint64_t>(); }

    /// generate random float
    static float next_float() { return next<float>(); }

    /// generate random double
    static double next_double() { return next<double>(); }

    /// generate random int32_t range [0, a)
    /// if a <= 0, return 0
    static int32_t gen_int(int32_t a) { return gen_int(0, a); }

    /// generate random int32_t range [a, b)
    /// if a >= b, return a
    static int32_t gen_int(int32_t a, int32_t b) {
        if (a >= b) return a;
        return next_uint32() % (b - a) + a;
    }

    /// generate double range [0, 1)
    static double gen_real() { return (next_uint64() >> 11) * (1.0 / 9007199254740992.0); }

private:
    template <class T>
    static T next() {
        T ret;
        if (!next_bytes(&ret, sizeof(T))) throw std::runtime_error("fetch next random bytes fail");
        return ret;
    }
};

class randomizer {
public:
    explicit randomizer(uint64_t n) : n_(n), d_(0, n_) {}

    /// return on the closed interval [0, n_]
    uint64_t operator()() { return d_(e); }

private:
    int n_;
    std::uniform_int_distribution<uint64_t> d_;

    static std::random_device r;
    static std::default_random_engine e;
};

inline std::string random_string(size_t size, const std::string& charset = "0123456789") {
    if (size == 0 || charset.empty()) return "";
    std::string str(size, 0);
    randomizer rand(charset.size() - 1);
    for (size_t i = 0; i < size; ++i) {
        str[i] = charset[rand()];
    }
    return str;
}

}  // namespace liph

#endif  // LIPH_NUMERIC_RANDOM_H_
