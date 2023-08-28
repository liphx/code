#include "liph/numeric/big_integer.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <type_traits>
#include <vector>

#include "liph/numeric/random.h"

#define BYTEMAX std::numeric_limits<byte>::max()

namespace liph {

namespace {

using byte = big_integer::byte;

// check str is like "0|[-][1-9][0-9]*" and set sign [0|1|-1]
bool check_str_is_number(const std::string& str, int& sign) {
    if (str.empty()) return false;
    sign = str[0] == '0' ? 0 : (str[0] == '-' ? -1 : 1);
    if (str == "0") return true;
    size_t i = str[0] == '-' ? 1 : 0;
    if (i >= str.length() || str[i] < '1' || str[i] > '9') return false;
    return std::all_of(str.begin() + i + 1, str.end(), [](char ch) { return ch >= '0' && ch <= '9'; });
}

// compare two vector, return 0 if equal, 1 if less and -1 if greater
int vector_cmp(const std::vector<byte>& vc1, const std::vector<byte>& vc2) {
    if (vc1.size() != vc2.size()) return vc1.size() < vc2.size() ? 1 : -1;
    // start from high position
    for (auto it1 = vc1.crbegin(), it2 = vc2.crbegin(); it1 != vc1.crend(); ++it1, ++it2) {
        if (*it1 > *it2)
            return -1;
        else if (*it1 < *it2)
            return 1;
    }
    return 0;
}

// compare two string(>= 0), return true if str1 < str2
bool str_cmp(const std::string& str1, const std::string& str2) {
    auto len1 = str1.length(), len2 = str2.length();
    return (len1 < len2) || (len1 == len2 && str1 < str2);
}

// add two str(>=0)
std::string str_add(const std::string& str1, const std::string& str2) {
    std::string ret;
    auto it1 = str1.crbegin(), it2 = str2.crbegin();

    int current = 0, need_carry = 0;
    while (it1 != str1.crend() || it2 != str2.crend()) {
        current = need_carry;
        need_carry = 0;

        if (it1 != str1.crend()) current += *it1++ - '0';
        if (it2 != str2.crend()) current += *it2++ - '0';

        if (current >= 10) {
            need_carry = 1;
            current -= 10;
        }

        ret += current + '0';
    }

    if (need_carry) ret += '1';

    std::reverse(ret.begin(), ret.end());
    return ret;
}

std::vector<byte> vector_add(const std::vector<byte>& vc1, const std::vector<byte>& vc2) {
    std::vector<byte> ret;
    ret.reserve(std::max(vc1.size(), vc2.size()) + 1);
    auto it1 = vc1.cbegin(), it2 = vc2.cbegin();
    int32_t current = 0;
    while (it1 != vc1.cend() || it2 != vc2.cend()) {
        if (it1 != vc1.cend()) current += *it1++;
        if (it2 != vc2.cend()) current += *it2++;
        ret.emplace_back(current % (BYTEMAX + 1));
        current /= (BYTEMAX + 1);
    }
    if (current > 0) ret.emplace_back(current);
    return ret;
}

std::vector<byte> vector_subtract(const std::vector<byte>& vc1, const std::vector<byte>& vc2, int& sign) {
    int cmp = vector_cmp(vc1, vc2);
    sign = -cmp;
    if (cmp == 0) return {};

    const std::vector<byte> *bigger = cmp == 1 ? &vc2 : &vc1;
    const std::vector<byte> *smaller = cmp == 1 ? &vc1 : &vc2;

    std::vector<byte> ret;
    auto it1 = bigger->cbegin(), it2 = smaller->cbegin();
    int32_t current = 0;
    while (it1 != bigger->cend()) {
        current += *it1++;
        if (it2 != smaller->cend()) current -= *it2++;
        if (current < 0) {
            ret.emplace_back(static_cast<byte>(current + BYTEMAX + 1));
            current = -1;
        } else {
            ret.emplace_back(static_cast<byte>(current));
            current = 0;
        }
    }
    while (ret.back() == 0) ret.pop_back();

    return ret;
}

// str1 - str2(str1, str2 >= 0)
std::string str_subtract(const std::string& str1, const std::string& str2) {
    if (str_cmp(str1, str2)) return '-' + str_subtract(str2, str1);

    auto it1 = str1.crbegin(), it2 = str2.crbegin();
    std::string str;
    int need_borrow = 0;
    while (it1 != str1.crend()) {
        int current = *it1++ - need_borrow;
        need_borrow = 0;
        if (it2 != str2.crend()) current -= (*it2++ - '0');

        if (current < '0') {
            current += 10;
            need_borrow = 1;
        }

        str += current;
    }

    int i = str.length() - 1;
    for (; i >= 0 && str[i] == '0'; i--)
        ;
    if (i < 0) return "0";

    std::string ret = std::string(str, 0, i + 1);
    std::reverse(ret.begin(), ret.end());
    return ret;
}

// str(>=0) multiply char('0'-'9')
std::string str_multiply_char(const std::string& str, char ch) {
    std::string ret;
    auto it = str.crbegin();
    int current = 0, carry = 0;
    while (it != str.crend()) {
        current = carry;
        carry = 0;
        current += (*it - '0') * (ch - '0');
        if (current >= 10) {
            carry = current / 10;
            current %= 10;
        }
        ret += current + '0';
        it++;
    }
    if (carry) ret += carry + '0';
    std::reverse(ret.begin(), ret.end());
    return ret;
}

// multiply two str(>=0)
std::string str_multiply(const std::string& str1, const std::string& str2) {
    if (str1 == "0" || str2 == "0") return "0";
    std::string ret = "0";
    size_t len = str2.length();
    for (size_t i = 0; i < len; i++)
        ret = str_add(ret, str_multiply_char(str1, str2[len - 1 - i]) + std::string(i, '0'));
    return ret;
}

std::vector<byte> vector_multiply(const std::vector<byte>& vc1, const std::vector<byte>& vc2) {
    assert(!vc1.empty() && !vc2.empty());
    std::vector<byte> ret;
    ret.reserve(vc1.size() + vc2.size());

    int32_t current = 0, idx = 0;
    for (size_t i = 0; i < vc1.size(); i++) {
        for (size_t j = 0; j < vc2.size(); j++) {
            idx = i + j;
            if (static_cast<int32_t>(ret.size()) < idx + 1) ret.resize(idx + 1);
            current += ret[i + j] + static_cast<int32_t>(vc1[i]) * static_cast<int32_t>(vc2[j]);
            ret[idx] = current % (BYTEMAX + 1);
            current /= (BYTEMAX + 1);
        }
        if (current > 0) {
            idx++;
            if (static_cast<int32_t>(ret.size()) < idx + 1) ret.resize(idx + 1);
            ret[idx] = current;
            current = 0;
        }
    }

    return ret;
}

// str1 / str2(str1 >= 0, str2 > 0)
std::string str_divide(const std::string& str1, const std::string& str2) {
    if (str_cmp(str1, str2)) return "0";

    int len = str2.length();
    int pos = len;  // pos of quotient
    if (str_cmp(std::string(str1, 0, len), str2)) {
        pos++;
    }

    std::string ret = "0", str = str1;

    for (char i = '9'; i > '0'; i--) {
        std::string tmp = str_multiply_char(str2, i);
        tmp += std::string(str.length() - pos, '0');
        if (!str_cmp(str, tmp)) {
            ret = str_add(ret, i + std::string(str1.length() - pos, '0'));
            str = str_subtract(str, tmp);
            ret = str_add(ret, str_divide(str, str2));
            break;
        }
    }

    return ret;
}

}  // namespace

////////////////////////////

big_integer::big_integer(long long n) {
    sign_ = n > 0 ? 1 : (n < 0 ? -1 : 0);
    unsigned long long num = (n < 0) ? static_cast<unsigned long long>(-n) : n;
    while (num != 0) {
        data_.emplace_back(num % (BYTEMAX + 1));
        num /= (BYTEMAX + 1);
    }
}

big_integer::big_integer(const big_integer& other) : sign_(other.sign_), data_(other.data_) {}

big_integer& big_integer::operator=(const big_integer& other) {
    if (this != &other) {
        sign_ = other.sign_;
        data_ = other.data_;
    }
    return *this;
}

big_integer::big_integer(big_integer&& other) : sign_(other.sign_), data_(std::move(other.data_)) { other.sign_ = 0; }

big_integer& big_integer::operator=(big_integer&& other) {
    if (this != &other) {
        sign_ = other.sign_;
        data_ = std::move(other.data_);
        other.sign_ = 0;
    }
    return *this;
}

big_integer::big_integer(const std::string& str) {
    int sign;
    if (!check_str_is_number(str, sign)) throw std::invalid_argument("invalid_argument");
    if (sign == 0) {
        *this = big_integer();
        return;
    }

    sign_ = sign;
    std::string s = sign == -1 ? std::string(str, 1) : str;
    std::string base = "256";  // BYTEMAX
    while (s != "0") {
        std::string q = str_divide(s, base);
        std::string r = str_subtract(s, str_multiply(base, q));
        std::istringstream iss(r);
        uint32_t i;
        iss >> i;
        data_.emplace_back(i);
        s = q;
    }
}

std::string big_integer::string() const {
    if (sign_ == 0) return "0";
    std::string str = "0";
    std::string factor = "1";
    std::string base = "256";  // BYTEMAX
    for (size_t i = 0; i < data_.size(); i++) {
        str = str_add(str, str_multiply(std::to_string(data_[i]), factor));
        factor = str_multiply(factor, base);
    }
    return (sign_ < 0) ? "-" + str : str;
}

bool big_integer::operator==(const big_integer& other) const { return sign_ == other.sign_ && data_ == other.data_; }

bool big_integer::operator!=(const big_integer& other) const { return !(*this == other); }

bool big_integer::operator<(const big_integer& other) const {
    if (sign_ != other.sign_) return sign_ < other.sign_;
    if (sign_ == 0) return false;
    int cmp = vector_cmp(data_, other.data_);
    return cmp != 0 && ((cmp == 1 && sign_ == 1) || (cmp == -1 && sign_ == -1));
}

bool big_integer::operator>(const big_integer& other) const {
    if (sign_ != other.sign_) return sign_ > other.sign_;
    if (sign_ == 0) return false;
    int cmp = vector_cmp(data_, other.data_);
    return cmp != 0 && ((cmp == 1 && sign_ == -1) || (cmp == -1 && sign_ == 1));
}

bool big_integer::operator<=(const big_integer& other) const {
    if (sign_ < other.sign_) return true;
    if (sign_ > other.sign_) return false;
    int cmp = vector_cmp(data_, other.data_);
    return cmp == 0 || (cmp == 1 && sign_ == 1) || (cmp == -1 && sign_ == -1);
}

bool big_integer::operator>=(const big_integer& other) const {
    if (sign_ < other.sign_) return false;
    if (sign_ > other.sign_) return true;
    int cmp = vector_cmp(data_, other.data_);
    return cmp == 0 || (cmp == 1 && sign_ == -1) || (cmp == -1 && sign_ == 1);
}

big_integer big_integer::operator+(const big_integer& other) const {
    if (other.zero()) return big_integer(*this);
    if (zero()) return big_integer(other);

    big_integer bi;
    if (sign_ == other.sign_) {
        bi.sign_ = sign_;
        bi.data_ = vector_add(data_, other.data_);
    } else {
        int ret_sign;
        bi.data_ = vector_subtract(data_, other.data_, ret_sign);
        bi.sign_ = sign_ == 1 ? ret_sign : -ret_sign;
    }
    return bi;
}

big_integer big_integer::operator-(const big_integer& other) const {
    if (sign_ == 0) return -other;
    if (other.sign_ == 0) return big_integer(*this);

    big_integer bi;
    if (sign_ != other.sign_) {
        bi.sign_ = sign_;
        bi.data_ = vector_add(data_, other.data_);
    } else {
        int ret_sign;
        bi.data_ = vector_subtract(data_, other.data_, ret_sign);
        bi.sign_ = sign_ == 1 ? ret_sign : -ret_sign;
    }

    return bi;
}

big_integer big_integer::operator-() const {
    big_integer bi(*this);
    bi.sign_ = -bi.sign_;
    return bi;
}

big_integer big_integer::operator*(const big_integer& other) const {
    if (zero() || other.zero()) return 0;
    big_integer bi;
    bi.sign_ = (sign_ == other.sign_) ? 1 : -1;
    bi.data_ = vector_multiply(data_, other.data_);
    return bi;
}

big_integer big_integer::operator/(const big_integer& other) const {
    if (other.zero()) throw std::invalid_argument("divide by zero");
    if (zero()) return 0;
    int cmp = vector_cmp(data_, other.data_);
    bool equal = cmp == 0;
    bool less = cmp == 1;
    if (less) return 0;
    if (equal) return big_integer(1);

    big_integer bi;
    big_integer r(*this);
    while (!(r < other)) {
        big_integer tmp(other);
        big_integer q(1);
        big_integer q2;
        while (tmp < r || tmp == r) {
            q2 = q;
            q = q * big_integer(2);
            tmp = other * q;
        }
        bi = bi + q2;
        r = r - q2 * other;
    }

    return bi;
}

big_integer big_integer::abs() const {
    big_integer bi(*this);
    if (bi.sign() < 0) {
        bi.sign_ = 1;
    }
    return bi;
}

bool big_integer::zero() const { return sign() == 0; }

int big_integer::sign() const { return sign_; }

big_integer big_integer::rand(uint64_t max_size) {
    big_integer bi;
    randomizer r(max_size);
    bi.sign_ = r() % 2 == 0 ? 1 : -1;
    bi.data_.resize(r() + 1);
    std::generate(bi.data_.begin(), bi.data_.end(), [&]() { return r(); });
    return bi;
}

}  // namespace liph

#undef BYTEMAX
