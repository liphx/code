#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

template <class T>
std::string to_string(const T& t) {
    return std::to_string(t);
}

template <class... Args>
std::string format(std::string_view fmt, const Args&...args) {
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
        } else if (state == 1 && fmt[i] == '{') {  // {{ for {
            state = 0;
            ret += '{';
        } else if (state == 2 && fmt[i] == '}') {  // }} for }
            state = 0;
            ret += '}';
        } else if (state == 1 && fmt[i] != '{' && fmt[i] != '}') {  // Illegal
            throw std::runtime_error("Illegal format string");
        } else if (state == 2 && fmt[i] != '}') {  // Illegal
            throw std::runtime_error("Illegal format string");
        } else if (state == 1 && fmt[i] == '}') {  // replacement field
            state = 0;
            int i = 0;
            (void)std::initializer_list<int>{(
                    [&ret, arg, &i](const auto& t) {
                        if (i++ == arg) ret += to_string(t);
                    }(args),
                    0)...};
            arg++;
        }
    }
    if (arg > arg_size) throw std::runtime_error("Missing format parameters");

    return ret;
}

int main() {
    std::cout << format("The answer is {}.", 42) << std::endl;
    // ignore redundant parameters
    std::cout << format("{} {}", 1, 2, 3) << std::endl;
}
