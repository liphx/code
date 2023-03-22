#ifndef LIPH_BIG_INTEGER_H_
#define LIPH_BIG_INTEGER_H_

#include <cstdint>
#include <string>
#include <vector>

namespace liph {

class BigInteger {
public:
    static const BigInteger Zero;

public:
    BigInteger() {}
    explicit BigInteger(long long n);
    explicit BigInteger(const std::string& str);
    BigInteger(const BigInteger& other) : sign_(other.sign_), data_(other.data_) {}

    BigInteger& operator=(const BigInteger& other) {
        sign_ = other.sign_;
        data_ = other.data_;
        return *this;
    }

    bool operator==(const BigInteger& other) const;
    bool operator<(const BigInteger& other) const;

    bool isZero() const { return sign_ == 0; }
    int sign() const { return sign_; }

    std::string toString() const;

    BigInteger operator+(const BigInteger& other) const;
    BigInteger operator-(const BigInteger& other) const;
    BigInteger operator*(const BigInteger& other) const;
    BigInteger operator/(const BigInteger& other) const;
    BigInteger operator-() const;

private:
    int sign_{0};
    std::vector<uint8_t> data_;
};

}  // namespace liph

#endif  // LIPH_BIG_INTEGER_H_
