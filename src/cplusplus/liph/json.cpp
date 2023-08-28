#include "liph/json.h"

#include <cassert>
#include <exception>
#include <sstream>
#include <string_view>

namespace liph {

namespace {

void skip_whitespace(std::string_view& sv);
bool eat_symbol(std::string_view& sv, std::string_view symbol);
json parse_value(std::string_view& sv);
json parse_number(std::string_view& sv);
json parse_string(std::string_view& sv);
json parse_array(std::string_view& sv);
json parse_object(std::string_view& sv);

void skip_whitespace(std::string_view& sv) {
    std::string_view::size_type pos = 0;
    while (pos < sv.size() && isspace(sv[pos])) pos++;
    sv = sv.substr(pos);
}

bool eat_symbol(std::string_view& sv, std::string_view symbol) {
    if (sv.compare(0, symbol.size(), symbol) != 0) return false;
    sv = sv.substr(symbol.size());
    return true;
}

json parse_value(std::string_view& sv) {
    skip_whitespace(sv);

    // literal
    if (eat_symbol(sv, "true")) return json(true);
    if (eat_symbol(sv, "false")) return json(false);
    if (eat_symbol(sv, "null")) return json();

    if (sv.empty()) throw std::runtime_error("parse value error");

    if (sv[0] == '"') return parse_string(sv);
    if (sv[0] == '{') return parse_object(sv);
    if (sv[0] == '[') return parse_array(sv);

    return parse_number(sv);
}

json parse_number(std::string_view& sv) {
    skip_whitespace(sv);
    size_t i = 0;
    bool use_double = false;
    while (i < sv.size()) {
        if (sv[i] == '.' || sv[i] == 'e' || sv[i] == 'E') {
            use_double = true;
            ++i;
        } else if (isdigit(sv[i]) || sv[i] == '-') {
            ++i;
        } else {
            break;
        }
    }
    std::string str(sv.substr(0, i));
    json ret;
    if (use_double) {
        ret = json(std::stod(str));
    } else {
        ret = json(static_cast<int64_t>(std::stoll(str)));
    }
    sv = sv.substr(i);
    return ret;
}

json parse_string(std::string_view& sv) {
    skip_whitespace(sv);

    if (sv.empty() || sv[0] != '"') throw std::runtime_error("parse string error");

    std::string_view::size_type pos = 1;
    int state = 0;

    json j("");
    std::string& str = j.string_ref();

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
    j.reset();
    throw std::runtime_error("parse string error");
}

json parse_object(std::string_view& sv) {
    skip_whitespace(sv);

    if (!eat_symbol(sv, "{")) throw std::runtime_error("parse object error");

    json j((std::unordered_map<std::string, json>()));

    while (true) {
        skip_whitespace(sv);
        if (eat_symbol(sv, "}")) return j;

        json s = parse_string(sv);
        skip_whitespace(sv);
        if (!eat_symbol(sv, ":")) {
            s.reset();
            throw std::runtime_error("parse object error");
        }

        json v = parse_value(sv);
        skip_whitespace(sv);
        j.object_ref()[s.string_ref()] = v;
        if (!eat_symbol(sv, ",")) {
            break;
        }
    }

    skip_whitespace(sv);
    if (eat_symbol(sv, "}")) return j;

    j.reset();
    throw std::runtime_error("parse object error");
}

json parse_array(std::string_view& sv) {
    skip_whitespace(sv);

    if (!eat_symbol(sv, "[")) throw std::runtime_error("parse array error");

    json j((std::vector<json>()));

    while (true) {
        skip_whitespace(sv);
        if (eat_symbol(sv, "]")) return j;

        json v = parse_value(sv);
        j.array_ref().emplace_back(v);
        skip_whitespace(sv);
        if (!eat_symbol(sv, ",")) {
            break;
        }
    }

    skip_whitespace(sv);
    if (eat_symbol(sv, "]")) return j;

    j.reset();
    throw std::runtime_error("parse array error");
}

}  // namespace

json::json() : type_(null) {}

json::json(bool b) : type_(boolean), bool_(b) {}

json::json(int32_t n) : type_(number), i64_(n) {}

json::json(int64_t n) : type_(number), i64_(n) {}

json::json(double d) : type_(number), is_double_(true), double_(d) {}

json::json(std::string str) : type_(string) { string_ = new std::string(std::move(str)); }

json::json(const char *str) {
    if (str) {
        type_ = string;
        string_ = new std::string(str);
    } else {
        type_ = null;
    }
}

json::json(const std::vector<json>& arr) : type_(array) { array_ = new std::vector<json>(arr); }

json::json(const std::unordered_map<std::string, json>& obj) : type_(object) {
    object_ = new std::unordered_map<std::string, json>(obj);
}

json::json(const json& other) : type_(other.type_) {
    switch (type_) {
    case null:
        break;
    case boolean:
        bool_ = other.bool_;
        break;
    case number:
        i64_ = other.i64_;
        break;
    case string:
        string_ = new std::string(*other.string_);
        break;
    case array:
        array_ = new std::vector<json>(*other.array_);
        break;
    case object:
        object_ = new std::unordered_map<std::string, json>(*other.object_);
        break;
    default:
        assert(0);
    }
}

json::json(json&& other) : type_(other.type_) {
    switch (type_) {
    case null:
        break;
    case boolean:
        bool_ = other.bool_;
        break;
    case number:
        i64_ = other.i64_;
        break;
    case string:
        string_ = new std::string(std::move(*other.string_));
        break;
    case array:
        array_ = new std::vector<json>(std::move(*other.array_));
        break;
    case object:
        object_ = new std::unordered_map<std::string, json>(std::move(*other.object_));
        break;
    default:
        assert(0);
    }
    other.reset();
}

json& json::operator=(const json& other) {
    if (this == &other) return *this;
    reset();
    type_ = other.type_;
    switch (type_) {
    case null:
        break;
    case boolean:
        bool_ = other.bool_;
        break;
    case number:
        i64_ = other.i64_;
        break;
    case string:
        string_ = new std::string(*other.string_);
        break;
    case array:
        array_ = new std::vector<json>(*other.array_);
        break;
    case object:
        object_ = new std::unordered_map<std::string, json>(*other.object_);
        break;
    default:
        assert(0);
    }

    return *this;
}

json& json::operator=(json&& other) {
    if (this == &other) return *this;
    reset();
    type_ = other.type_;
    switch (type_) {
    case null:
        break;
    case boolean:
        bool_ = other.bool_;
        break;
    case number:
        i64_ = other.i64_;
        break;
    case string:
        string_ = new std::string(*other.string_);
        break;
    case array:
        array_ = new std::vector<json>(*other.array_);
        break;
    case object:
        object_ = new std::unordered_map<std::string, json>(*other.object_);
        break;
    default:
        assert(0);
    }
    other.reset();
    return *this;
}

json::~json() { reset(); }

void json::clear() {
    switch (type_) {
    case string:
        string_->clear();
        break;
    case array:
        array_->clear();
        break;
    case object:
        object_->clear();
        break;
    default:
        throw std::logic_error("type error");
    }
}

void json::reset() {
    switch (type_) {
    case string:
        delete string_;
        break;
    case array:
        delete array_;
        break;
    case object:
        delete object_;
        break;
    default:
        break;
    }
    type_ = null;
    is_double_ = false;
    string_ = nullptr;
}

json json::parse(const std::string& str) {
    std::string_view sv(str);
    skip_whitespace(sv);
    json j = parse_value(sv);
    skip_whitespace(sv);
    if (sv.empty()) return j;
    throw std::runtime_error("parse json error");
}

const json& json::at(const std::string& key) const {
    if (type_ != object) {
        throw std::logic_error("type error");
    }
    return object_->at(key);
}

json& json::at(const std::string& key) { return const_cast<json&>(static_cast<const json&>(*this).at(key)); }

const json& json::at(const std::size_t& pos) const {
    if (type_ != array) {
        throw std::logic_error("type error");
    }
    return array_->at(pos);
}

json& json::at(const std::size_t& pos) { return const_cast<json&>(static_cast<const json&>(*this).at(pos)); }

bool json::operator==(const json& other) const {
    if (type_ != other.type_) return false;
    if (type_ == null) return true;
    if (type_ == boolean) return bool_ == other.bool_;
    if (type_ == number) return i64_ == other.i64_;
    if (type_ == string) return *string_ == *other.string_;
    if (type_ == array) return *array_ == *other.array_;
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
    case object: {
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
    case array: {
        ss << '[';
        std::string sep;
        for (const auto& i : *array_) {
            ss << sep << i.to_string();
            sep = ", ";
        }
        ss << ']';
        break;
    }
    case string:
        ss << '"' << *string_ << '"';
        break;
    case number:
        if (is_double_)
            ss << double_;
        else
            ss << i64_;
        break;
    case boolean:
        ss << std::boolalpha << bool_;
        break;
    case null:
        ss << "null";
        break;
    default:
        break;
    }

    return ss.str();
}

bool& json::bool_ref() {
    if (type_ != boolean) throw std::logic_error("type error");
    return bool_;
}

double& json::double_ref() {
    if (type_ != number) throw std::logic_error("type error");
    return double_;
}

std::string& json::string_ref() {
    if (type_ != string) throw std::logic_error("type error");
    return *string_;
}

std::vector<json>& json::array_ref() {
    if (type_ != array) throw std::logic_error("type error");
    return *array_;
}

std::unordered_map<std::string, json>& json::object_ref() {
    if (type_ != object) throw std::logic_error("type error");
    return *object_;
}

}  // namespace liph
