#include "liph/file.h"

#include <filesystem>
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

bool path_exists(const std::string& pathname) {
    std::error_code ec;
    return std::filesystem::exists(pathname, ec);
}

std::vector<std::string> list_files(const std::string& pathname) {
    std::vector<std::string> ret;
    try {
        for (auto& p : std::filesystem::recursive_directory_iterator(pathname)) ret.emplace_back(p.path().string());
    } catch (const std::filesystem::filesystem_error&) {
        return {};
    }
    std::sort(ret.begin(), ret.end());
    return ret;
}

}  // namespace liph
