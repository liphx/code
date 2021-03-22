#include "BigInteger.h"
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <climits>

namespace {

// 字符串相加
std::string add(std::string str1, std::string str2)
{
    std::string ret;
    std::reverse(str1.begin(), str1.end());
    std::reverse(str2.begin(), str2.end());
    int len1 = str1.length(), len2 = str2.length();
    int flag = 0; // 是否进位
    int i;
    for (i = 0; i < len1 && i < len2; i++) {
        int tmp = flag + str1[i] + str2[i] - '0' - '0'; // 减两个'0'
        if (tmp >= 10) {    // 需进位
            flag = 1;
            tmp -= 10;
        } else {            // 不进位，复位flag
            flag = 0;
        }
        ret += (tmp + '0'); // 加'0'
    }

    while (i < len1) {
        int tmp = flag + str1[i] - '0';  // 减1个'0'
        if (tmp >= 10) {
            flag = 1;
            tmp -= 10;
        } else {
            flag = 0;
        }
        ret += (tmp + '0'); // 加'0'
        i++;
    }

    while (i < len2) {
        int tmp = flag + str2[i] - '0'; 
        if (tmp >= 10) {
            flag = 1;
            tmp -= 10;
        } else {
            flag = 0;
        }
        ret += (tmp + '0');
        i++;
    }

    if (flag == 1) {
        ret += '1';
    }

    std::reverse(ret.begin(), ret.end());
    return ret;
}

// 字符串相减，str代表的整数满足 str1 > str2 > 0
std::string subtract(std::string str1, std::string str2)
{
    std::string ret;
    std::reverse(str1.begin(), str1.end());
    std::reverse(str2.begin(), str2.end());
    int len1 = str1.length(), len2 = str2.length();
    int flag = 0; // 是否借位
    int i = 0;
    while (i < len2) {
        int tmp = str1[i] - str2[i] - flag;
        if (tmp < 0) { // 借位
            flag = 1;
            tmp += 10;
        } else {
            flag = 0;
        }
        ret += (tmp + '0');
        i++;
    }
    while (i < len1) {
        int tmp = str1[i] - flag - '0';
        if (tmp < 0) {
            flag = 1;
            tmp += 10;
        } else {
            flag = 0;
        }
        ret += (tmp + '0');
        i++;
    }

    std::reverse(ret.begin(), ret.end());

    // 去掉前导0
    int len = ret.length(), j;
    for (j = 0; j < len && ret[j] == '0'; j++) ; 
    ret = std::string(ret, j);

    return ret;
}

// 字符串乘字符
std::string multiply_char(std::string str, char ch, int base)
{
    std::string ret;
    std::reverse(str.begin(), str.end());
    int i = 0, n = str.length();
    int flag = 0;
    while (i < n) {
        int tmp = flag + (str[i] - '0') * (ch - '0');
        if (tmp >= 10) {
            flag = tmp / 10;
            tmp %= 10;
        } else {
            flag = 0;
        }
        ret += (tmp + '0');
        i++;
    }

    if (flag) {
        ret += (flag + '0');
    }

    std::reverse(ret.begin(), ret.end());
    // ret * 10^base
    ret = ret + std::string(base, '0');
    return ret;
}

// 字符串相乘，str均不为0
std::string multiply(std::string str1, std::string str2)
{

    std::string ret = "0";
    int len2 = str2.length();
    for (int i = 0; i < len2; i++) {
        ret = add(ret, multiply_char(str1, str2[len2 - 1 - i], i));
    }
    return ret;
}

// 比较两个字符串大小，不带符号，若str1 < str2，返回true
bool str_cmp(std::string str1, std::string str2)
{
    int len1 = str1.length(), len2 = str2.length();
    return (len1 < len2) || (len1 == len2 && str1 < str2);
}

// 字符串相除
std::string divide(std::string str1, std::string str2)
{
    if (str_cmp(str1, str2))
        return "0";

    std::string ret = "0";
    int len1 = str1.length(), len2 = str2.length();
    std::string tmp = std::string(str1, 0, len2); // 取和str2等长的字符串，判断商的位置
    int i; // 表示商的位置
    if (str_cmp(tmp, str2)) {
        i = len2 + 1;
    } else {
        i = len2;
    }

    for (int j = 9; j >= 0; j--) {
        tmp = multiply_char(str2, j + '0', str1.length() - i);
        if (!str_cmp(str1, tmp)) {
            ret = add(ret, (char)(j + '0') + std::string(str1.length() - i, '0'));
            str1 = subtract(str1, tmp);
            ret = add(ret, divide(str1, str2));
            break;
        }
    }
    
    return ret;
}


} // 匿名namespace

// 构造函数 default: n = 0
BigInteger::BigInteger(long long n)
{
    std::ostringstream os;
    os << n;
    if (n == 0) {
        this->sign = 0;
        this->data = os.str();
    } else if (n > 0) {
        this->sign = 1;
        this->data = os.str();
    } 
    /* else if (n == LLONG_MIN) { // 考虑long long 最小值
        *this = -(BigInteger(LLONG_MAX) + BigInteger(1));
    } */ 
    else {
        this->sign = -1;
        this->data = std::string(os.str(), 1); // 不存储符号位
    }
}

// 构造函数，若str不能转成BigInteger（如包含非法字符），将抛出异常
BigInteger::BigInteger(std::string str)
{
    if (str == "") {
        this->sign = 0;
        this->data = "0";
        return;
    }

    // 检查str符号
    int sign_flag = 1; // 为-1表示str带有前导的 '-'
    int i = 0;
    if (str[i] == '-') {
        sign_flag = -1;
        i++;
    } else if (str[i] == '+') {
        i++;
    }
    int start = i;

    // 检查str能否转成BigInteger，若不能将抛异常
    int err = 0;  // 为1表示str不能转成BigInteger
    int len = str.length();
    while (i < len) {
        if (str[i] < '0' || str[i] > '9') {
            err = 1;
            break;
        }
        i++;
    }
    if (err) {
        throw(1);
    }

    // 去掉str前导的0
    while (start < len) {
        if (str[start] == '0') {
            start++;
        } else {
            break;
        }
    }

    if (start == len) { // 表示str除符号位外全是0
        this->sign = 0;
        this->data = "0";
        return;
    }

    this->sign = sign_flag;
    this->data = std::string(str, start);
}

// 拷贝构造函数
BigInteger::BigInteger(const BigInteger &bi)
{
    this->sign = bi.sign;
    this->data = bi.data;
}

// 赋值运算符
BigInteger& BigInteger::operator=(const BigInteger &bi)
{
    this->sign = bi.sign;
    this->data = bi.data;
    return *this;
}

// 析构函数，无资源需要释放
BigInteger::~BigInteger()
{
    ;
}

// 转成字符串
std::string BigInteger::toString() const
{
    if (this->sign == 0) {
        return std::string("0");
    } else if (this->sign > 0) {
        return this->data;
    } else {
        return std::string("-") + this->data;
    }
}

// 输出流
std::ostream &operator<<(std::ostream &out, const BigInteger &bi)
{
    out << bi.toString();
    return out;
}

// 重载 ==
bool BigInteger::operator==(const BigInteger &bi) const
{
    return (this->sign == bi.sign) && (this->data == bi.data);
}

// 重载 <
bool BigInteger::operator<(const BigInteger &bi) const
{
    // 异号
    if (this->sign < bi.sign)
        return true;
    
    // 同号
    int len_a = this->data.length();
    int len_b = bi.data.length();

    if (this->sign == 1) { // 正
        return (len_a < len_b) || (len_a == len_b && this->data < bi.data);
    } else if (this->sign == -1) { // 负
        return (len_a > len_b) || (len_a == len_b && this->data > bi.data);
    } else { // 零
        return false;
    }
}

// 使用< == 重载其它比较运算符
bool BigInteger::operator>(const BigInteger &bi) const
{
    return !(*this < bi || *this == bi);
}

bool BigInteger::operator!=(const BigInteger &bi) const
{
    return !(*this == bi);
}

bool BigInteger::operator<=(const BigInteger &bi) const
{
    return (*this < bi || *this == bi);
}

bool BigInteger::operator>=(const BigInteger &bi) const
{
    return !(*this < bi);
}

// 重载一元负号
BigInteger BigInteger::operator-() const
{
    BigInteger ret(*this);
    ret.sign = -ret.sign; // 仅需要修改符号
    return ret;
}

// 重载 +
BigInteger BigInteger::operator+(const BigInteger &bi) const
{
    // 仅处理同号，异号由 - 处理
    if (this->sign == 0 && bi.sign == 0) { // 0 + 0
        return BigInteger(0);
    }

    if (this->sign >= 0 && bi.sign >=0) { // 非负 + 非负（不会同为0）
        BigInteger ret;
        ret.sign = 1;
        ret.data = ::add(this->data, bi.data);
        return ret;
    } else if (this->sign <= 0 && bi.sign <= 0) { // 非正 + 非正（不会同为0）
        return -((-*this) + (-bi));
    } else { // 正 + 负（均不会为0），交由减法处理
        return (this->sign > 0) ? (*this - (-bi)) : (bi - (-*this));
    }
}

// 重载 -
BigInteger BigInteger::operator-(const BigInteger &bi) const
{
    // 相等
    if (*this == bi)
        return BigInteger(0);

    // 其中一个为0
    if (this->sign == 0)
        return -bi;
    if (bi.sign == 0)
        return *this;

    // 一正一负
    if (this->sign < 0 && bi.sign > 0)
        return -(-*this + bi);
    if (this->sign > 0 && bi.sign < 0)
        return *this + (-bi);

    // 同为正负
    BigInteger ret;
    if (this->sign > 0 && *this > bi) { // 同正且前者大
        ret.sign = 1;
        ret.data = ::subtract(this->data, bi.data);
        return ret;
    } else if (this->sign > 0 && *this < bi) { // 同正且后者大
        ret.sign = -1;
        ret.data = ::subtract(bi.data, this->data);
        return ret;
    } else { // 同负
        return -(-*this - (-bi));
    }
}

// 重载 *
BigInteger BigInteger::operator*(const BigInteger &bi) const
{
    if (this->sign == 0 || bi.sign == 0) // 至少一个为0
        return BigInteger(0);
    
    BigInteger ret;
    ret.sign = (this->sign == bi.sign) ? 1 : -1;
    ret.data = ::multiply(this->data, bi.data);
    return ret;
}

// 重载 /，除数为0将抛异常
BigInteger BigInteger::operator/(const BigInteger &bi) const
{
    if (bi.sign == 0) { // 除数为0
        throw(2);
    }
    if (this->sign == 0) { // 被除数为0
        return BigInteger(0);
    }

    if (this->abs() < bi.abs()) { // 被除数的绝对值小于除数
        return BigInteger(0);
    }

    BigInteger ret;
    ret.sign = (this->sign == bi.sign) ? 1 : -1;
    ret.data = ::divide(this->data, bi.data);
    return ret;
}

// 重载 %，除数为0将抛异常
BigInteger BigInteger::operator%(const BigInteger &bi) const
{
    if (bi.sign == 0) { // 除数为0
        throw(2);
    }
    return *this - ((*this / bi) * bi);
}

BigInteger BigInteger::abs() const
{
    BigInteger ret(*this);
    if (ret.sign < 0)
        ret.sign = -ret.sign;
    return ret;
}

namespace {
BigInteger _gcd(const BigInteger &a, const BigInteger &b)
{
        if (a < b) 
            return _gcd(b, a);

        if (b == 0) {
            return a;
        }
        return _gcd(b, a % b);
}

} // namespace

// 以这个函数为入口，a, b > 0
BigInteger BigInteger::gcd(const BigInteger &a, const BigInteger &b)
{
    if (a <= 0 || b <= 0) {
        throw(3);
    }

    return ::_gcd(a, b);
}

BigInteger BigInteger::lcm(const BigInteger &a, const BigInteger &b)
{
    if (a <= 0 || b <= 0) {
        throw(3);
    }

    return a * b / gcd(a, b);
}
