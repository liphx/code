#include "big_integer.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>
#include <type_traits>
#include <vector>

namespace liph {

const BigInteger BigInteger::Zero = BigInteger();

namespace {

template <class T>
bool cmp_vector_(const std::vector<T>& vc1, const std::vector<T>& vc2, bool *equal) {
    *equal = false;
    if (vc1.size() != vc2.size()) return vc1.size() < vc2.size();

    // 高位在后面
    for (auto it1 = vc1.crbegin(), it2 = vc2.crbegin(); it1 != vc1.crend(); it1++, it2++) {
        if (*it1 > *it2)
            return false;
        else if (*it1 < *it2)
            return true;
    }
    *equal = true;
    return false;
}

// compare two string(>= 0), return true if str1 < str2
inline bool str_cmp_(const std::string& str1, const std::string& str2) {
    int len1 = str1.length(), len2 = str2.length();
    return (len1 < len2) || (len1 == len2 && str1 < str2);
}

// add two str(>=0)
std::string str_add_(const std::string& str1, const std::string& str2) {
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

std::vector<uint8_t> vector_add_(const std::vector<uint8_t>& vc1, const std::vector<uint8_t>& vc2) {
    std::vector<uint8_t> ret;
    ret.reserve(std::max(vc1.size(), vc2.size()) + 1);
    auto it1 = vc1.cbegin(), it2 = vc2.cbegin();
    int32_t current = 0;
    while (it1 != vc1.cend() || it2 != vc2.cend()) {
        if (it1 != vc1.cend()) current += *it1++;
        if (it2 != vc2.cend()) current += *it2++;
        ret.emplace_back(current % (UINT8_MAX + 1));
        current /= (UINT8_MAX + 1);
    }
    if (current > 0) ret.emplace_back(current);
    return ret;
}

std::vector<uint8_t> vector_subtract_(const std::vector<uint8_t>& vc1, const std::vector<uint8_t>& vc2, int *sign) {
    bool equal;
    bool less = cmp_vector_(vc1, vc2, &equal);
    if (equal) {
        *sign = 0;
        return std::vector<uint8_t>();
    }
    const std::vector<uint8_t> *bigger, *smaller;
    if (less) {
        *sign = -1;
        bigger = &vc2;
        smaller = &vc1;
    } else {
        *sign = 1;
        bigger = &vc1;
        smaller = &vc2;
    }

    std::vector<uint8_t> ret;
    auto it1 = bigger->cbegin(), it2 = smaller->cbegin();
    int32_t current = 0;
    while (it1 != bigger->cend()) {
        current += *it1++;
        if (it2 != smaller->cend()) current -= *it2++;
        if (current < 0) {
            ret.emplace_back(static_cast<uint8_t>(current + UINT8_MAX + 1));
            current = -1;
        } else {
            ret.emplace_back(static_cast<uint8_t>(current));
            current = 0;
        }
    }
    while (ret.back() == 0) ret.pop_back();

    return ret;
}

// str1 - str2(str1, str2 >= 0)
std::string str_subtract_(const std::string& str1, const std::string& str2) {
    if (str_cmp_(str1, str2)) return '-' + str_subtract_(str2, str1);

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
std::string str_multiply_char_(const std::string& str, char ch) {
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
std::string str_multiply_(const std::string& str1, const std::string& str2) {
    if (str1 == "0" || str2 == "0") return "0";
    std::string ret = "0";
    size_t len = str2.length();
    for (size_t i = 0; i < len; i++) {
        ret = str_add_(ret, str_multiply_char_(str1, str2[len - 1 - i]) + std::string(i, '0'));
    }
    return ret;
}

std::vector<uint8_t> vector_multiply_(const std::vector<uint8_t>& vc1, const std::vector<uint8_t>& vc2) {
    assert(!vc1.empty() && !vc2.empty());
    std::vector<uint8_t> ret;
    ret.reserve(vc1.size() + vc2.size());

    int32_t current = 0, idx = 0;
    for (size_t i = 0; i < vc1.size(); i++) {
        for (size_t j = 0; j < vc2.size(); j++) {
            idx = i + j;
            if (static_cast<int32_t>(ret.size()) < idx + 1) ret.resize(idx + 1);
            current += ret[i + j] + static_cast<int32_t>(vc1[i]) * static_cast<int32_t>(vc2[j]);
            ret[idx] = current % (UINT8_MAX + 1);
            current /= (UINT8_MAX + 1);
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
std::string str_divide_(const std::string& str1, const std::string& str2) {
    if (str_cmp_(str1, str2)) return "0";

    int len = str2.length();
    int pos = len;  // 表示商的位置
    if (str_cmp_(std::string(str1, 0, len), str2)) {
        pos++;
    }

    std::string ret = "0", str = str1;

    for (char i = '9'; i > '0'; i--) {
        std::string tmp = str_multiply_char_(str2, i);
        tmp += std::string(str.length() - pos, '0');
        if (!str_cmp_(str, tmp)) {
            ret = str_add_(ret, i + std::string(str1.length() - pos, '0'));
            str = str_subtract_(str, tmp);
            ret = str_add_(ret, str_divide_(str, str2));
            break;
        }
    }

    return ret;
}

}  // namespace

////////////////////////////

BigInteger::BigInteger(long long n) {
    if (n == 0) {
        sign_ = 0;
        return;
    }

    sign_ = (n > 0) ? 1 : -1;
    unsigned long long num = (n < 0) ? static_cast<unsigned long long>(-n) : n;
    while (num != 0) {
        data_.emplace_back(num % (UINT8_MAX + 1));
        num /= (UINT8_MAX + 1);
    }
}

// 0|[-][1-9][0-9]*
static bool check_str_is_number(const std::string& str, int *sign) {
    if (str.empty()) return false;
    if (str == "0") {
        *sign = 0;
        return true;
    }

    size_t i = 0;
    if (str[i] == '-') {
        *sign = -1;
        i++;
    } else {
        *sign = 1;
    }

    if (i >= str.length() || str[i] < '1' || str[i] > '9') return false;

    for (; i < str.length(); i++)
        if (str[i] < '0' || str[i] > '9') return false;

    return true;
}

BigInteger::BigInteger(const std::string& str) {
    int sign;
    if (!check_str_is_number(str, &sign)) throw std::invalid_argument("invalid_argument");
    if (sign == 0) {
        *this = BigInteger();
        return;
    }

    this->sign_ = sign;
    std::string s;
    if (sign == -1)
        s = std::string(str, 1);
    else
        s = str;

    std::string base = "256";
    while (s != "0") {
        std::string q = str_divide_(s, base);
        std::string r = str_subtract_(s, str_multiply_(base, q));
        std::istringstream iss(r);
        uint32_t i;
        iss >> i;
        data_.emplace_back(i);
        s = q;
    }
}

std::string BigInteger::toString() const {
    if (sign_ == 0) return "0";
    std::string str = "0";
    std::string factor = "1";
    std::string base = "256";  // UINT8_MAX
    for (size_t i = 0; i < data_.size(); i++) {
        str = str_add_(str, str_multiply_(std::to_string(data_[i]), factor));
        factor = str_multiply_(factor, base);
    }
    return (sign_ < 0) ? "-" + str : str;
}

bool BigInteger::operator==(const BigInteger& other) const {
    if (sign_ != other.sign_) return false;
    return data_ == other.data_;
}

bool BigInteger::operator<(const BigInteger& other) const {
    if (sign_ != other.sign_) return sign_ < other.sign_;

    if (sign_ == 0) return false;

    bool equal;
    bool ret = cmp_vector_(data_, other.data_, &equal);
    if (equal) return false;
    return (sign_ == 1 && ret) || (sign_ == -1 && !ret);
}

BigInteger BigInteger::operator+(const BigInteger& other) const {
    if (other.isZero()) return BigInteger(*this);
    if (isZero()) return BigInteger(other);

    BigInteger bi;
    if (sign_ == other.sign_) {
        bi.sign_ = sign_;
        bi.data_ = vector_add_(data_, other.data_);
    } else {
        int ret_sign;
        bi.data_ = vector_subtract_(data_, other.data_, &ret_sign);
        bi.sign_ = sign_ == 1 ? ret_sign : -ret_sign;
    }
    return bi;
}

BigInteger BigInteger::operator-(const BigInteger& other) const {
    if (sign_ == 0) return -other;
    if (other.sign_ == 0) return BigInteger(*this);

    BigInteger bi;
    if (sign_ != other.sign_) {
        bi.sign_ = sign_;
        bi.data_ = vector_add_(data_, other.data_);
    } else {
        int ret_sign;
        bi.data_ = vector_subtract_(data_, other.data_, &ret_sign);
        bi.sign_ = sign_ == 1 ? ret_sign : -ret_sign;
    }

    return bi;
}

BigInteger BigInteger::operator-() const {
    BigInteger bi(*this);
    bi.sign_ = -bi.sign_;
    return bi;
}

BigInteger BigInteger::operator*(const BigInteger& other) const {
    if (isZero() || other.isZero()) return BigInteger();

    BigInteger bi;
    bi.sign_ = (sign_ == other.sign_) ? 1 : -1;
    bi.data_ = vector_multiply_(data_, other.data_);
    return bi;
}

BigInteger BigInteger::operator/(const BigInteger& other) const {
    if (other.isZero()) throw std::invalid_argument("divide by zero");

    if (isZero()) return BigInteger();

    bool equal;
    bool less = cmp_vector_(data_, other.data_, &equal);
    if (less) return BigInteger();
    if (equal) return BigInteger(1);

    BigInteger bi;
    BigInteger r(*this);
    while (!(r < other)) {
        BigInteger tmp(other);
        BigInteger q(1);
        BigInteger q2;
        while (tmp < r || tmp == r) {
            q2 = q;
            q = q * BigInteger(2);
            tmp = other * q;
        }
        bi = bi + q2;
        r = r - q2 * other;
    }

    return bi;
}

}  // namespace liph
