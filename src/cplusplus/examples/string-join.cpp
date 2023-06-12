#include <iostream>
#include <sstream>
#include <vector>

template <class Container>
std::string join(const Container& data, const std::string& sep = " ") {
    std::ostringstream os;
    std::string tmp;
    for (auto it = data.begin(); it != data.end(); ++it) {
        os << tmp << *it;
        tmp = sep;
    }
    return os.str();
}

template <class Container>
std::string join2(const Container& data, const std::string& sep = " ") {
    std::ostringstream os;
    for (auto it = data.begin(); it != data.end(); ++it) {
        os << ((it == data.begin()) ? "" : sep) << *it;
    }
    return os.str();
}

template <class Container>
std::string join3(const Container& data, const std::string& sep = " ") {
    std::ostringstream os;
    bool first = true;
    for (auto it = data.begin(); it != data.end(); ++it) {
        if (!first) {
            os << sep;
        }
        first = false;
        os << *it;
    }
    return os.str();
}

void print(const int *arr, int n) {
    for (int i = 0; i < n; i++) std::cout << arr[i] << ",\n"[i == n - 1];
}

int main() {
    const std::vector<int> data{1, 2, 4, 1, 9};
    std::cout << join(data, ",") << std::endl;
    std::cout << join2(data, ",") << std::endl;
    std::cout << join3(data, ",") << std::endl;
    print(data.data(), data.size());
}
