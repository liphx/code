#include "liph/file.h"

#include <fstream>

namespace liph {

bool read_file(const std::string& pathname, std::string& output) {
    std::ifstream is{pathname, std::ios::binary | std::ios::ate};
    if (is.is_open()) {
        auto size = is.tellg();
        std::string str(size, '\0');
        is.seekg(0);
        is.read(&str[0], size);
        if (is.good()) {
            output.append(str);
            return true;
        }
    }
    return false;
}

bool write_file(const std::string& pathname, const std::string& str) {
    std::ofstream out(pathname);
    if (out.is_open()) {
        out.write(str.data(), str.size());
        return out.good();
    }
    return false;
}

}  // namespace liph
