#ifndef LIPH_NUMERIC_RANDOM_H_
#define LIPH_NUMERIC_RANDOM_H_

#include <cstdint>
#include <random>
#include <stdexcept>
#include <string>
#include <string_view>

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

    /// generate random int32_t range [0, n)
    /// if n <= 0, return 0
    static int32_t gen_int(int32_t n) { return n <= 0 ? 0 : (next_uint32() / 2) % n; }

    /// generate random int32_t range [a, b)
    /// if a >= b, return a
    static int32_t gen_int(int32_t a, int32_t b) { return a + gen_int(b - a); }

    /// generate double range [0, 1)
    static double gen_real() { return (next_uint64() >> 11) * (1.0 / 9007199254740992.0); }

    /// generate random string of length size from charset
    static std::string gen_str(size_t size, std::string_view charset = "0123456789");

private:
    template <class T>
    static T next() {
        T ret;
        if (!next_bytes(&ret, sizeof(T))) throw std::runtime_error("fetch next random bytes fail");
        return ret;
    }
};

#if 0
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
#endif

class rand {
public:
#if 0
    rand(uint32_t seed = 1) { ::srand(seed); }
    uint32_t operator()() { return ::rand(); }
#endif

    rand(uint32_t seed = 1) : seed_(seed & 0x7fffffffu) {
        if (seed_ == 0 || seed_ == 2147483647L) {
            seed_ = 1;
        }
    }

    /// generate random number range [min(), max()]
    uint32_t operator()() {
        static const uint32_t M = 2147483647L;
        static const uint64_t A = 16807;
        // seed_ = (seed_ * A) % M
        uint64_t product = seed_ * A;
        seed_ = static_cast<uint32_t>((product >> 31) + (product & M));
        if (seed_ > M) {
            seed_ -= M;
        }
        return seed_;
    }

    static uint32_t min() { return 1; }            // seed: 1407677000
    static uint32_t max() { return 2147483646L; }  // seed: 739806647

private:
    uint32_t seed_;
};

}  // namespace liph

#endif  // LIPH_NUMERIC_RANDOM_H_
