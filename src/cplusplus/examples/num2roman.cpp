#include <cassert>

#include "liph/liph.h"

const std::map<int, char> num_roman_map = {
        {1, 'I'}, {5, 'V'}, {10, 'X'}, {50, 'L'}, {100, 'C'}, {500, 'D'}, {1000, 'M'}};
const std::map<char, int> roman_num_map = {
        {'I', 1}, {'V', 5}, {'X', 10}, {'L', 50}, {'C', 100}, {'D', 500}, {'M', 1000}};

std::string num2roman(int num) {
    std::string ret;
    if (num <= 0) return ret;
    for (auto it = num_roman_map.crbegin(); it != num_roman_map.crend(); ++it) {
        ret.append(num / it->first, it->second);
        num %= it->first;
    }
    return ret;
}

int roman2num(const std::string& roman) {
    int ret = 0;
    int pre = num_roman_map.rbegin()->first;
    for (char ch : roman) {
        auto it = roman_num_map.find(ch);
        if (it == roman_num_map.end()) return -1;
        int value = it->second;
        ret += value;
        if (value > pre) ret -= pre + pre;
        pre = value;
    }
    return ret;
}

void check(int num, const std::string& roman) {
    /* std::cout << num << ", " << roman << std::endl; */
    assert(num2roman(num) == roman);
    assert(num == roman2num(roman));
}

int main() {
    check(1, "I");
    check(3, "III");
    /* check(9, "IX"); */
    /* check(45, "XLV"); */
    /* check(99, "XCIX"); */
}
