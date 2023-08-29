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

size_t eat_digits(std::string_view& sv, std::string& str) {
    size_t i = 0;
    while (i < sv.size() && isdigit(sv[i])) {
        str += sv[i];
        ++i;
    }
    sv = sv.substr(i);
    return i;
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
    std::string str;
    size_t i = 0;
    if (eat_symbol(sv, "-")) {
        str += '-';
        i = 1;
    }
    auto n = eat_digits(sv, str);
    if (n == 0 || (n > 1 && str[i] == '0')) throw std::runtime_error("parse number error");
    bool use_double = false;
    if (eat_symbol(sv, ".")) {  // fraction
        use_double = true;
        str += '.';
        if (eat_digits(sv, str) == 0) throw std::runtime_error("parse number error");
    }
    if (eat_symbol(sv, "e") || eat_symbol(sv, "E")) {  // exponent
        use_double = true;
        str += 'e';
        if (eat_symbol(sv, "+"))
            str += '+';
        else if (eat_symbol(sv, "-"))
            str += '-';
        if (eat_digits(sv, str) == 0) throw std::runtime_error("parse number error");
    }

    if (!use_double) {
        try {
            return json(static_cast<int64_t>(std::stoll(str)));
        } catch (std::out_of_range&) {  // not catch std::invalid_argument if no conversion
        }
    }
    return json(std::stod(str));
}

json parse_string(std::string_view& sv) {
    skip_whitespace(sv);

    if (sv.empty() || sv[0] != '"') throw std::runtime_error("parse string error");

    std::string_view::size_type pos = 1;
    int state = 0;

    json j("");
    std::string& str = j.string_ref();

    while (pos < sv.size()) {
        if (state == 0 && sv[pos] == '"') {  // end
            sv = sv.substr(pos + 1);
            return j;
        } else if (state == 0 && sv[pos] != '\\') {  //  not escape
            str += sv[pos];
            pos++;
        } else if (state == 0 && sv[pos] == '\\') {  // escape
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
                str += "\\u";
                state = 2;  // unicode
                break;
            default:
                goto ERROR;
            }
            pos++;
            if (state != 2) state = 0;  // back to normal
        } else if (state == 2) {        // 4 hex digits
            for (size_t i = 0; i < 4; ++i) {
                static auto is_hex = [](char ch) {
                    return (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
                };
                if (pos + i >= sv.size() || !is_hex(sv[pos + i])) {
                    goto ERROR;
                }
                str += sv[pos + i];
            }
            pos += 4;
            state = 0;
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

json::json(value_type type) : type_(type) {
    switch (type_) {
    case string:
        string_ = new std::string;
        break;
    case array:
        array_ = new std::vector<json>;
        break;
    case object:
        object_ = new std::unordered_map<std::string, json>;
        break;
    default:
        break;
    }
}

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

json::value_type json::type() const { return type_; }

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

json& json::operator[](const std::string& key) {
    if (type_ != object) {
        throw std::logic_error("type error");
    }
    return (*object_)[key];
}

const json& json::at(std::size_t pos) const {
    if (type_ != array) {
        throw std::logic_error("type error");
    }
    return array_->at(pos);
}

json& json::at(std::size_t pos) { return const_cast<json&>(static_cast<const json&>(*this).at(pos)); }

json& json::operator[](std::size_t pos) {
    if (type_ != array) {
        throw std::logic_error("type error");
    }
    array_->resize(std::max(pos + 1, array_->size()));
    return (*array_)[pos];
}

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

int64_t& json::i64_ref() {
    if (type_ != number) throw std::logic_error("type error");
    return i64_;
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
