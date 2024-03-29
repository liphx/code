#ifndef LIPH_NUMERIC_BIG_INTEGER_H_
#define LIPH_NUMERIC_BIG_INTEGER_H_

#include <cstdint>
#include <string>
#include <vector>

namespace liph {

class big_integer {
public:
    using byte = unsigned char;

public:
    big_integer(long long n = 0);
    explicit big_integer(const std::string& str);
    big_integer(const big_integer& other);
    big_integer& operator=(const big_integer& other);
    big_integer(big_integer&& other);
    big_integer& operator=(big_integer&& other);

    int sign() const;
    bool zero() const;
    std::string string() const;

    bool operator==(const big_integer& other) const;
    bool operator!=(const big_integer& other) const;
    bool operator<(const big_integer& other) const;
    bool operator<=(const big_integer& other) const;
    bool operator>(const big_integer& other) const;
    bool operator>=(const big_integer& other) const;

    big_integer operator+(const big_integer& other) const;
    big_integer operator-(const big_integer& other) const;
    big_integer operator*(const big_integer& other) const;
    big_integer operator/(const big_integer& other) const;
    big_integer operator-() const;
    big_integer abs() const;

    static big_integer rand(int max_size = 128);

private:
    int sign_{0};
    std::vector<byte> data_;
};

}  // namespace liph

inline liph::big_integer operator""_bi(const char *str, std::size_t size) {
    liph::big_integer bi(std::string(str, size));
    return bi;
}

#endif  // LIPH_NUMERIC_BIG_INTEGER_H_
