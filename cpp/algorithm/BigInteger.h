#ifndef _BIG_INTEGER_
#define _BIG_INTEGER_

#include <string>
#include <iostream>

// 仅用string实现大整数
class BigInteger {
public:
    BigInteger(long long n = 0);
    BigInteger(std::string str);
    BigInteger(const BigInteger &bi);
    BigInteger& operator=(const BigInteger &bi);

    BigInteger operator-() const;
    BigInteger operator+(const BigInteger &bi) const;
    BigInteger operator-(const BigInteger &bi) const;
    BigInteger operator*(const BigInteger &bi) const;
    BigInteger operator/(const BigInteger &bi) const;
    BigInteger operator%(const BigInteger &bi) const;

    bool operator==(const BigInteger &bi) const;
    bool operator<(const BigInteger &bi) const;
    bool operator>(const BigInteger &bi) const;
    bool operator!=(const BigInteger &bi) const;
    bool operator<=(const BigInteger &bi) const;
    bool operator>=(const BigInteger &bi) const;

    std::string toString() const; // 转成字符串，包含符号
    ~BigInteger();
    friend std::ostream &operator<<(std::ostream &out, const BigInteger &bi);

    static BigInteger gcd(const BigInteger &a, const BigInteger &b); // 最大公因数
    static BigInteger lcm(const BigInteger &a, const BigInteger &b); // 最小公倍数
private:
    int sign;           // 符号位，1表示正数，-1表示负数，0代表0
    std::string data;   // 以字符串形式存储大整数，其中不包含符号位，有效的格式为[1-9][0-9]* | 0
    BigInteger abs() const;
};

#endif
