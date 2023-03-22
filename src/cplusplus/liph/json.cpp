#include "json.h"

#include <cassert>
#include <exception>
#include <sstream>
#include <string_view>

namespace liph {

namespace {

void skip_whitespace_(std::string_view& sv);
bool eat_symbol_(std::string_view& sv, std::string_view symbol);
json parse_value_(std::string_view& sv);
json parse_number_(std::string_view& sv);
json parse_string_(std::string_view& sv);
json parse_array_(std::string_view& sv);
json parse_object_(std::string_view& sv);

void skip_whitespace_(std::string_view& sv) {
    std::string_view::size_type pos = 0;
    while (pos < sv.size() && isspace(sv[pos])) pos++;
    sv = sv.substr(pos);
}

bool eat_symbol_(std::string_view& sv, std::string_view symbol) {
    if (sv.compare(0, symbol.size(), symbol) != 0) return false;
    sv = sv.substr(symbol.size());
    return true;
}

json parse_value_(std::string_view& sv) {
    skip_whitespace_(sv);

    // literal
    if (eat_symbol_(sv, "true")) return json(true);
    if (eat_symbol_(sv, "false")) return json(false);
    if (eat_symbol_(sv, "null")) return json();

    if (sv.empty()) throw std::exception();

    if (sv[0] == '"') return parse_string_(sv);
    if (sv[0] == '{') return parse_object_(sv);
    if (sv[0] == '[') return parse_array_(sv);

    return parse_number_(sv);
}

json parse_number_(std::string_view& sv) {
    skip_whitespace_(sv);

    std::istringstream ss(sv.data());
    double num;
    ss >> num;
    if (ss.fail()) throw std::exception();

    auto pos = (ss.tellg() != -1) ? (std::string_view::size_type)ss.tellg() : sv.size();
    sv = sv.substr(pos);

    return json(num);
}

json parse_string_(std::string_view& sv) {
    skip_whitespace_(sv);

    if (sv.empty() || sv[0] != '"') throw std::exception();

    std::string_view::size_type pos = 1;
    int state = 0;

    json j("");
    std::string& str = j.get_string();

    while (pos < sv.size()) {
        if (state == 0 && sv[pos] == '"') {  // 终止
            sv = sv.substr(pos + 1);
            return j;
        } else if (state == 0 && sv[pos] != '\\') {  // 未转义
            str += sv[pos];
            pos++;
        } else if (state == 0 && sv[pos] == '\\') {  // 进入转义
            state = 1;
            pos++;
        } else if (state == 1) {
            switch (sv[pos]) {
            case '"':
                str += '"';
                break;
            case '\\':
                str += '\\';
                break;
            case '/':
                str += '/';
                break;
            case 'b':
                str += '\b';
                break;
            case 'f':
                str += '\f';
                break;
            case 'n':
                str += '\n';
                break;
            case 'r':
                str += '\r';
                break;
            case 't':
                str += '\t';
                break;
            case 'u':
                state = 2;  // unicode
                break;
            default:
                goto ERROR;
            }
            pos++;
            if (state != 2) state = 0;  // back to normal
        } else if (state == 2) {        // 4 hex digits
            // todo
            goto ERROR;
        }
    }

ERROR:
    j.clear();
    throw std::exception();
}

json parse_object_(std::string_view& sv) {
    skip_whitespace_(sv);

    if (!eat_symbol_(sv, "{")) throw std::exception();

    json j((std::unordered_map<std::string, json>()));

    while (true) {
        skip_whitespace_(sv);
        if (eat_symbol_(sv, "}")) return j;

        json s = parse_string_(sv);
        skip_whitespace_(sv);
        if (!eat_symbol_(sv, ":")) {
            s.clear();
            throw std::exception();
        }

        json v = parse_value_(sv);
        skip_whitespace_(sv);
        j.get_object()[s.get_string()] = v;
        if (!eat_symbol_(sv, ",")) {
            break;
        }
    }

    skip_whitespace_(sv);
    if (eat_symbol_(sv, "}")) return j;

    j.clear();
    throw std::exception();
}

json parse_array_(std::string_view& sv) {
    skip_whitespace_(sv);

    if (!eat_symbol_(sv, "[")) throw std::exception();

    json j((std::vector<json>()));

    while (true) {
        skip_whitespace_(sv);
        if (eat_symbol_(sv, "]")) return j;

        json v = parse_value_(sv);
        j.get_array().emplace_back(v);
        skip_whitespace_(sv);
        if (!eat_symbol_(sv, ",")) {
            break;
        }
    }

    skip_whitespace_(sv);
    if (eat_symbol_(sv, "]")) return j;

    j.clear();
    throw std::exception();
}

}  // namespace

json::json() : type_(Null) {}

json::json(bool b) : type_(Bool), bool_(b) {}

json::json(double n) : type_(Number), number_(n) {}

json::json(const std::string& str) : type_(String) { string_ = new std::string(str); }

json::json(const char *str) {
    if (str == nullptr) {
        type_ = Null;
    } else {
        type_ = String;
        string_ = new std::string(str);
    }
}

json::json(const std::vector<json>& arr) : type_(Array) { array_ = new std::vector<json>(arr); }

json::json(const std::unordered_map<std::string, json>& obj) : type_(Object) {
    object_ = new std::unordered_map<std::string, json>(obj);
}

json::json(const json& other) : type_(other.type_) {
    switch (type_) {
    case Null:
        break;
    case Bool:
        bool_ = other.bool_;
        break;
    case Number:
        number_ = other.number_;
        break;
    case String:
        string_ = new std::string(*other.string_);
        break;
    case Array:
        array_ = new std::vector<json>(*other.array_);
        break;
    case Object:
        object_ = new std::unordered_map<std::string, json>(*other.object_);
        break;
    default:
        assert(0);
    }
}

void json::clear() {
    switch (type_) {
    case String:
        delete string_;
        break;
    case Array:
        delete array_;
        break;
    case Object:
        delete object_;
        break;
    default:
        break;
    }

    type_ = Null;
    string_ = nullptr;
}

json& json::operator=(const json& other) {
    if (this == &other) return *this;

    clear();

    type_ = other.type_;
    switch (type_) {
    case Null:
        break;
    case Bool:
        bool_ = other.bool_;
        break;
    case Number:
        number_ = other.number_;
        break;
    case String:
        string_ = new std::string(*other.string_);
        break;
    case Array:
        array_ = new std::vector<json>(*other.array_);
        break;
    case Object:
        object_ = new std::unordered_map<std::string, json>(*other.object_);
        break;
    default:
        assert(0);
    }

    return *this;
}

json::~json() { clear(); }

json json::parse(const std::string& str) {
    std::string_view sv(str);
    skip_whitespace_(sv);
    json j = parse_value_(sv);
    skip_whitespace_(sv);
    if (sv.empty()) return j;
    throw std::exception();
}

const json& json::at(const std::string& key) const {
    if (type_ != Object) {
        throw std::exception();
    }
    return object_->at(key);
}

json& json::at(const std::string& key) { return const_cast<json&>(static_cast<const json&>(*this).at(key)); }

const json& json::at(const std::size_t& pos) const {
    if (type_ != Array) {
        throw std::exception();
    }
    return array_->at(pos);
}

json& json::at(const std::size_t& pos) { return const_cast<json&>(static_cast<const json&>(*this).at(pos)); }

bool json::operator==(const json& other) const {
    if (type_ != other.type_) return false;
    if (type_ == Null) return true;
    if (type_ == Bool) return bool_ == other.bool_;
    if (type_ == Number) return number_ == other.number_;
    if (type_ == String) return *string_ == *other.string_;
    if (type_ == Array) return *array_ == *other.array_;
    for (const auto& x : *object_) {
        if (other.object_->count(x.first) == 0 || x.second != other.object_->at(x.first)) {
            return false;
        }
    }
    return true;
}

bool json::operator!=(const json& other) const { return !(*this == other); }

std::string json::to_string() const {
    std::ostringstream ss;
    switch (type_) {
    case Object: {
        ss << '{';
        std::string sep;
        for (const auto& i : *object_) {
            ss << sep;
            ss << '"' << i.first << "\": " << i.second.to_string();
            sep = ", ";
        }
        ss << '}';
        break;
    }
    case Array: {
        ss << '[';
        std::string sep;
        for (const auto& i : *array_) {
            ss << sep << i.to_string();
            sep = ", ";
        }
        ss << ']';
        break;
    }
    case String:
        ss << '"' << *string_ << '"';
        break;
    case Number:
        ss << number_;
        break;
    case Bool:
        ss << std::boolalpha << bool_;
        break;
    case Null:
        ss << "null";
        break;
    default:
        break;
    }

    return ss.str();
}

bool& json::get_bool() {
    if (type_ != Bool) throw std::exception();
    return bool_;
}

double& json::get_number() {
    if (type_ != Number) throw std::exception();
    return number_;
}

std::string& json::get_string() {
    if (type_ != String) throw std::exception();
    return *string_;
}

std::vector<json>& json::get_array() {
    if (type_ != Array) throw std::exception();
    return *array_;
}

std::unordered_map<std::string, json>& json::get_object() {
    if (type_ != Object) throw std::exception();
    return *object_;
}

}  // namespace liph
