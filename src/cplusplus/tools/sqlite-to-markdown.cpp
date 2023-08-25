#include <iomanip>

#include "liph/liph.h"

using u8 = unsigned char;

struct rule {
    std::vector<u8> chars;
    int num;

    std::string string() const { return std::string((char *)chars.data(), chars.size()); }
};

std::vector<rule> rules = {
        {{208, 129}, 1},
        {{224, 185, 140}, 0},
        {{226, 132, 162}, 1},
};

struct rule_range {
    std::vector<std::vector<u8>> chars;
    int num;
};

std::vector<rule_range> rule_ranges = {
        {{{194}, {179, 189}}, 1},
        {{{195}, {168, 188}}, 1},
        {{{208}, {161, 189}}, 1},
        {{{209}, {130, 142}}, 1},
        {{{216}, {162, 179}}, 1},
        {{{217}, {129, 136}}, 1},
        {{{218}, {134, 134}}, 1},
        {{{219}, {140, 140}}, 1},
        {{{224}, {184}, {129, 178}}, 1},
        {{{224}, {185}, {128, 140}}, 1},
        {{{226}, {128}, {148, 153}}, 1},
        {{{226}, {133}, {160, 162}}, 1},
};

void print() {
    for (const auto& rule : rules) {
        std::cout << rule.string() << std::endl;
    }
}

int utf8_len(const std::string& str) {
    int ret = 0;
    size_t i = 0;
    const u8 *begin = (const u8 *)str.data();
    while (i < str.size()) {
        try {
            bool match = false;
            for (const auto& rule : rules) {
                if (std::equal(rule.chars.begin(), rule.chars.end(), begin + i)) {
                    i += rule.chars.size();
                    ret += rule.num;
                    match = true;
                    break;
                }
            }
            if (match) continue;

            for (const auto& rule : rule_ranges) {
                if (std::equal(
                            rule.chars.begin(), rule.chars.end(), begin + i, [](const std::vector<u8>& range, u8 ch) {
                                return ch >= range.front() && ch <= range.back();
                            })) {
                    i += rule.chars.size();
                    ret += rule.num;
                    match = true;
                    break;
                }
            }
            if (match) continue;

            u8 leader = static_cast<u8>(str.at(i));
            if (leader >= 0 && leader <= 127) {
                i++;
                ret++;
            } else if (leader >= 194 && leader <= 223) {
                i += 2;
                ret += 2;
            } else if (leader >= 224 && leader <= 239) {
                i += 3;
                ret += 2;
            } else if (leader >= 240 && leader <= 247) {
                i += 4;
                ret += 2;
            } else {
                throw 0;
            }
        } catch (...) {
            std::cerr << "not utf-8 code" << std::endl;
            return 0;
        }
    }
    return ret;
}

int main(int argc, char **argv) {
    liph::flags flags;
    flags.register_string_flag("db");
    flags.register_string_flag("sql");
    if (!flags.parse_flags(argc, &argv) || flags.string_ref("db").empty() || flags.string_ref("sql").empty()) {
        std::cerr << flags.help();
        return 1;
    }
    liph::sqlite db(flags.string_ref("db"));
    auto ans = db.query(flags.string_ref("sql"));
    if (ans.empty()) {
        std::cerr << "Result is empty" << std::endl;
        return 1;
    }
    std::vector<int> vc(ans[0].size());
    for (size_t i = 0; i < ans.size(); ++i) {
        for (size_t j = 0; j < ans[i].size(); ++j) {
            int len = utf8_len(ans[i][j]);
            vc[j] = std::max({vc[j], len, 3});
        }
    }
    // header
    for (size_t j = 0; j < ans[0].size(); ++j) {
        std::cout << "| " << ans[0][j] << std::string(vc[j] + 1 - utf8_len(ans[0][j]), ' ');
    }
    std::cout << "|" << std::endl;
    // --
    for (size_t j = 0; j < ans[0].size(); ++j) {
        std::cout << "| " << std::string(vc[j], '-') << " ";
    }
    std::cout << "|" << std::endl;
    // body
    for (size_t i = 1; i < ans.size(); ++i) {
        for (size_t j = 0; j < ans[i].size(); ++j) {
            std::cout << "| " << ans[i][j] << std::string(vc[j] + 1 - utf8_len(ans[i][j]), ' ');
        }
        std::cout << "|" << std::endl;
    }
}
