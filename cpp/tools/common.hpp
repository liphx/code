#ifndef COMMON_H_
#define COMMON_H_

#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>

template<typename T>
ostream& operator<<(ostream& o, const vector<T>& vc)
{
    o << "[";
    string sep;
    for (auto x: vc) {
        o << sep << x;
        sep = ", ";
    }
    o << "]";
    return o;
}

template<class T>
T fromString(const std::string& s)
{
    std::istringstream is(s);
    T t;
    is >> t;
    return t;
}

template<class T>
std::string toString(const T& t)
{
    std::ostringstream os;
    os << t;
    return os.str();
}

template<class... Args>
void print(const Args&... args)
{
    std::initializer_list<int> { ([](auto i){ std::cout << i << ' '; }(args), 0)... };
    std::cout << std::endl;
}

class format_error: public std::runtime_error {
public:
    format_error(std::string msg): std::runtime_error(msg) {};
};

// 参数不合法抛出异常，有多余参数不是错误
template<class... Args>
std::string format(std::string fmt, const Args&... args)
{
    std::string ret;
    int len = fmt.length();

    int state = 0;
    int arg = 0;
    int arg_size = sizeof...(args);

    for (int i = 0; i < len; i++) {
        if (state == 0 && fmt[i] != '{' && fmt[i] != '}') {
            ret += fmt[i];
        } else if (state == 0 && fmt[i] == '{') {
            state = 1;
        } else if (state == 0 && fmt[i] == '}') {
            state = 2;
        } else if (state == 1 && fmt[i] == '{') { // {{ 转义 {
            state = 0;
            ret += '{';
        } else if (state == 2 && fmt[i] == '}') { // }} 转义 }
            state = 0;
            ret += '}';
        } else if (state == 1 && fmt[i] != '{' && fmt[i] != '}') { // 非法
            throw format_error("Illegal format string");
        } else if (state == 2 && fmt[i] != '}') { // 非法
            throw format_error("Illegal format string");
        } else if (state == 1 && fmt[i] == '}') { // 替换域
            state = 0;
            int i = 0;
            std::initializer_list<int> { ([&ret, arg, &i](auto t) { 
                if (i++ == arg) ret += toString(t);
            }(args), 0)... };
            arg++;
        }
    }
    if (arg > arg_size)
        throw format_error("Missing format parameters");

    return ret;
}

#endif // COMMON_H_
