#ifndef LIPH_NUMERIC_RANDOM_H_
#define LIPH_NUMERIC_RANDOM_H_

#include <cstdint>
#include <random>

namespace liph {

class randomizer {
public:
    explicit randomizer(uint64_t n) : n_(n), d_(0, n_) {}

    // return on the closed interval [0, n_]
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
