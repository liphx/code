#include <iomanip>

#include "liph/liph.h"

int utf8_len(const std::string& str) {
    int ret = 0;
    int i = 0;
    while (i < str.size()) {
        unsigned char leader = static_cast<unsigned char>(str[i]);
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
            goto err;
        }
    }
    if (i != str.size()) {
        goto err;
    }
    return ret;
err:
    std::cerr << "not utf-8 code" << std::endl;
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " db sql" << std::endl;
        return 1;
    }
    liph::Sqlite db(argv[1]);
    auto ans = db.query(argv[2]);
    std::vector<int> vc(ans[0].size());
    for (int i = 0; i < ans.size(); ++i) {
        for (int j = 0; j < ans[i].size(); ++j) {
            int len = utf8_len(ans[i][j]);
            vc[j] = std::max(vc[j], len);
        }
    }
    /* P(vc); */
    for (int i = 0; i < ans.size(); ++i) {
        for (int j = 0; j < ans[i].size(); ++j) {
            std::cout << "| " << ans[i][j] << std::string(vc[j] + 1 - utf8_len(ans[i][j]), ' ');
        }
        std::cout << "|" << std::endl;
    }
}
