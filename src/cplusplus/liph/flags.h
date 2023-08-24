#ifndef LIPH_FLAGS_H_
#define LIPH_FLAGS_H_

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "liph/string.h"

namespace liph {

class flags {
public:
    flags() = default;

    bool register_bool_flag(const std::string& name, bool default_value = false);
    bool register_int32_flag(const std::string& name, int32_t default_value = 0);
    bool register_double_flag(const std::string& name, double default_value = 0.0);
    bool register_string_flag(const std::string& name, std::string default_value = "");

    // parse command line and remove registered flags
    bool parse_flags(int argc, char ***argv);

    bool has_flag(const std::string& name) const;

    bool bool_ref(const std::string& name);
    int32_t& int32_ref(const std::string& name);
    double& double_ref(const std::string& name);
    std::string& string_ref(const std::string& name);

    std::string help() const;

private:
    enum flag_type { flag_bool, flag_int32, flag_double, flag_string };
    struct flag_info {
        flag_type type;
        size_t idx;
    };
    std::unordered_map<std::string, flag_info> registered_flags_;
    std::vector<bool> values_bool_;
    std::vector<int32_t> values_int32_;
    std::vector<double> values_double_;
    std::vector<std::string> values_string_;

    bool parse_{false};
    std::vector<char *> new_argv_;
};

}  // namespace liph

#endif  // LIPH_FLAGS_H_
