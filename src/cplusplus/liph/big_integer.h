#ifndef LIPH_BIG_INTEGER_H_
#define LIPH_BIG_INTEGER_H_

#include <cstdint>
#include <string>
#include <vector>

namespace liph {

class big_integer {
public:
    big_integer(long long n = 0);

    explicit big_integer(const std::string& str);

    big_integer(const big_integer& other) : sign_(other.sign_), data_(other.data_) {}

    big_integer& operator=(const big_integer& other) {
        sign_ = other.sign_;
        data_ = other.data_;
        return *this;
    }

    big_integer(big_integer&& other) : sign_(other.sign_), data_(std::move(other.data_)) { other.sign_ = 0; }

    big_integer& operator=(big_integer&& other) {
        sign_ = other.sign_;
        data_ = std::move(other.data_);
        other.sign_ = 0;
        return *this;
    }

    bool operator==(const big_integer& other) const;
    bool operator<(const big_integer& other) const;

    bool is_zero() const { return sign() == 0; }
    int sign() const { return sign_; }

    std::string to_string() const;

    big_integer operator+(const big_integer& other) const;
    big_integer operator-(const big_integer& other) const;
    big_integer operator*(const big_integer& other) const;
    big_integer operator/(const big_integer& other) const;
    big_integer operator-() const;

private:
    int sign_{0};
    std::vector<uint8_t> data_;
};

}  // namespace liph

#endif  // LIPH_BIG_INTEGER_H_
