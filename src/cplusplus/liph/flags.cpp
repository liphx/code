#include "liph/flags.h"

namespace liph {

namespace {

bool check_flag_name(const std::string& name) {
    return !name.empty() && std::all_of(name.begin(), name.end(), [](char ch) { return isalnum(ch); });
}

}  // namespace

bool flags::register_bool_flag(const std::string& name, bool default_value) {
    if (!check_flag_name(name) || registered_flags_.contains(name)) return false;
    registered_flags_[name] = {flag_bool, values_bool_.size()};
    values_bool_.emplace_back(default_value);
    return true;
}

bool flags::register_int32_flag(const std::string& name, int default_value) {
    if (!check_flag_name(name) || registered_flags_.contains(name)) return false;
    registered_flags_[name] = {flag_int32, values_int32_.size()};
    values_int32_.emplace_back(default_value);
    return true;
}

bool flags::register_string_flag(const std::string& name, std::string default_value) {
    if (!check_flag_name(name) || registered_flags_.contains(name)) return false;
    registered_flags_[name] = {flag_string, values_string_.size()};
    values_string_.emplace_back(std::move(default_value));
    return true;
}

bool flags::parse_flags(int argc, char ***argv) {
    assert(argc > 0 && argv && *argv);

    // only parse once
    if (parse_) return false;
    parse_ = true;

    // [0] for program name, start from [1]
    int i = 1;
    std::vector<char *> args(argc - 1);
    std::generate(args.begin(), args.end(), [&]() { return (*argv)[i++]; });
    auto it = args.begin();
    while (it != args.end()) {
        char *str = *it;
        std::string name;
        // if startswith "--" then startswith "-" must true
        if (startswith(*it, "--")) {
            name = str + 2;
        } else if (startswith(*it, "-")) {
            name = str + 1;
        } else {
            new_argv_.emplace_back(*it);
            ++it;
            continue;
        }
        if (name.empty()) return false;
        auto info_it = registered_flags_.find(name);
        if (info_it == registered_flags_.end()) return false;
        const auto& [type, idx] = info_it->second;
        ++it;
        if (it == args.end()) return false;
        std::string value = *it;
        switch (type) {
        case flag_bool:
            if (value == "true") {
                values_bool_[idx] = true;
            } else if (value == "false") {
                values_bool_[idx] = false;
            } else {
                return false;
            }
            break;
        case flag_int32:
            values_int32_[idx] = std::stoi(value);
            break;
        case flag_string:
            values_string_[idx] = value;
            break;
        }
        ++it;
    }
    new_argv_.emplace_back(nullptr);  // argv[argc] is NULL
    *argv = new_argv_.data();
    return true;
}

bool flags::has_flag(const std::string& name) const { return registered_flags_.contains(name); }

bool flags::bool_ref(const std::string& name) {
    const auto& info = registered_flags_.at(name);
    if (info.type != flag_bool) throw 0;
    return values_bool_[info.idx];
}

int32_t& flags::int32_ref(const std::string& name) {
    const auto& info = registered_flags_.at(name);
    if (info.type != flag_int32) throw 0;
    return values_int32_[info.idx];
}

std::string& flags::string_ref(const std::string& name) {
    const auto& info = registered_flags_.at(name);
    if (info.type != flag_string) throw 0;
    return values_string_[info.idx];
}

std::string flags::help() const {
    std::string msg = "flags:\n";
    for (const auto& [name, info] : registered_flags_) {
        msg += "  --" + name + ": ";
        switch (info.type) {
        case flag_bool:
            msg += values_bool_[info.idx] ? "true" : "false";
            break;
        case flag_int32:
            msg += std::to_string(values_int32_[info.idx]);
            break;
        case flag_string:
            msg += values_string_[info.idx];
        }
        msg += "\n";
    }
    return msg;
}

}  // namespace liph
