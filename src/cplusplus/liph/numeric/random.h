#ifndef LIPH_NUMERIC_RANDOM_H_
#define LIPH_NUMERIC_RANDOM_H_

#include <cstdint>
#include <random>

namespace liph {

class randomizer {
public:
    explicit randomizer(uint64_t n) : n_(n), d_(0, n_) {}

    uint64_t operator()() { return d_(e); }

private:
    int n_;
    std::uniform_int_distribution<uint64_t> d_;

    static std::random_device r;
    static std::default_random_engine e;
};

}  // namespace liph

#endif  // LIPH_NUMERIC_RANDOM_H_
