#ifndef JSON_HPP_
#define JSON_HPP_

#include <string>
#include <vector>
#include <map>
#include <sstream>

namespace liph {

class json {
public:
    enum type { Object, Array, String, Number, Bool, Null };

    json(): type_(Null) {}
    json(bool b): type_(Bool), bool_(b) {}
    json(double n): type_(Number), number_(n) {}
    json(std::string str): type_(String), string_(str) {}
    json(const char *str);
    json(std::vector<json> arr): type_(Array), array_(arr) {}
    json(std::map<std::string, json> obj): type_(Object), object_(obj) {}

    static json parse(const std::string& str);
    std::string dump() const;
    json& at(const std::string& key);
    const json& at(const std::string& key) const;
    json& at(const std::size_t& pos);
    const json& at(const std::size_t& pos) const;
    json& operator[](const std::string& key);
    json& operator[](const std::size_t pos);

    bool operator==(const json& other) const;
    bool operator!=(const json& other) const;

private:
    static void skip_whilespace(const std::string& str, std::string::size_type& start);
    static bool eat_symbol(const std::string& str, std::string::size_type& start, const std::string& symbol);
    static json parse_value(const std::string& str, std::string::size_type& start);
    static json parse_object(const std::string& str, std::string::size_type& start);
    static json parse_array(const std::string& str, std::string::size_type& start);
    static json parse_string(const std::string& str, std::string::size_type& start);
    static json parse_number(const std::string& str, std::string::size_type& start);
    
    type type_;
    std::map<std::string, json> object_;
    std::vector<json> array_;
    std::string string_;
    double number_;
    bool bool_;
};

json::json(const char *str) {
    if (str == nullptr) {
        type_ = Null;
    } else {
        type_ = String;
        string_ = std::string(str);
    }
}

json& json::at(const std::string& key)
{
    if (type_ != Object) {
        throw std::exception();
    }
    return object_.at(key);
}

const json& json::at(const std::string& key) const
{
    if (type_ != Object) {
        throw std::exception();
    }
    return object_.at(key); 
}

json& json::at(const std::size_t& pos)
{
    if (type_ != Array) {
        throw std::exception();
    }
    return array_.at(pos);
}

const json& json::at(const std::size_t& pos) const
{
    if (type_ != Array) {
        throw std::exception();
    }
    return array_.at(pos);
}

json& json::operator[](const std::string& key)
{
    if (type_ == Null)
        type_ = Object;
    if (type_ != Object) {
        throw std::exception();
    }
    if (object_.count(key) == 0)
        object_[key] = json();
    return object_[key];
}

json& json::operator[](const std::size_t pos)
{
    if (type_ == Null)
        type_ = Array;
    if (type_ != Array) {
        throw std::exception();
    }
    return array_[pos];
}

bool json::operator==(const json& other) const
{
    if (type_ != other.type_)  return false;
    if (type_ == Null)      return true;
    if (type_ == Bool)      return bool_ == other.bool_;
    if (type_ == Number)    return number_ == other.number_;
    if (type_ == String)    return string_ == other.string_;
    if (type_ == Array)     return array_ == other.array_;
    for (auto x: object_) {
        if (other.object_.count(x.first) == 0 || x.second != other.object_.at(x.first)) {
            return false;
        }
    }
    return true;
}

bool json::operator!=(const json& other) const
{
    return !(*this == other);
}

json json::parse(const std::string& str)
{
    std::string::size_type start = 0;
    skip_whilespace(str, start);
    json j = parse_value(str, start);
    if (j.type_ != Object && j.type_ != Array)
        throw std::exception();
    skip_whilespace(str, start);
    if (start != str.size())
        throw std::exception();

    return j;
}

void json::skip_whilespace(const std::string& str, std::string::size_type& start)
{
    while (start < str.length()) {
        if (::isblank(str[start])) start++;
        else break;
    }
}

bool json::eat_symbol(const std::string& str, std::string::size_type& start, const std::string& symbol)
{
    if (start >= str.length()) return false;
    if (str.length() - start < symbol.length()) return false;
    if (symbol == std::string(str, start, symbol.length())) {
        start += symbol.length();
        return true;
    }
    return false;
}

json json::parse_number(const std::string& str, std::string::size_type& start)
{
    std::istringstream ss(std::string(str, start));
    double num;
    ss >> num;
    if (ss.fail()) {
        throw std::exception();
    }
    if (ss.tellg() != -1) start += ss.tellg();
    else start = str.size();
    return json(num);
}

json json::parse_string(const std::string& str, std::string::size_type& start)
{
    json j;
    j.type_ = String;
    int state = 0;
    start++;
    while (start < str.length()) {
        if (state == 0 && str[start] == '"') { // 终止
            start++;
            return j;
        }
        if (state == 0 && str[start] != '\\') { // 未转义
            j.string_ += str[start];
            start++;
        } else if (state == 0 && str[start] == '\\') { // 进入转义
            state = 1;
            start++;
        } else if (state == 1) {
            switch (str[start]) {
            case '"':
                j.string_ += '"';
                break;
            case '\\':
                j.string_ += '\\';
                break;
            case '/':
                j.string_ += '/';
                break;
            case 'b':
                j.string_ += '\b';
                break;
            case 'f':
                j.string_ += '\f';
                break;
            case 'n':
                j.string_ += '\n';
                break;
            case 'r':
                j.string_ += '\r';
                break;
            case 't':
                j.string_ += '\t';
                break;
            case 'u':
                state = 2;  // unicode
                break;
            default:
                throw std::exception();
            }
            start++;
            if (state != 2)
                state = 0; // back to normal
        } else if (state == 2) { // 4 hex digits
            // todo
        }
    }
    throw std::exception();
}

json json::parse_object(const std::string& str, std::string::size_type& start)
{
    json j;
    j.type_ = Object;
    start++;
    skip_whilespace(str, start);
    if (start >= str.length())
        throw std::exception();
    if (str[start] == '}') {
        start++;
        return j;
    }
    while (true) {
        skip_whilespace(str, start);
        json s = parse_string(str, start);
        skip_whilespace(str, start);
        if (!eat_symbol(str, start, ":")) {
            throw std::exception();
        }
        json v = parse_value(str, start);
        skip_whilespace(str, start);
        j.object_.insert(std::make_pair(s.string_, v));
        if (!eat_symbol(str, start, ",")) {
            break;
        }
    }
    if (start >= str.length())
        throw std::exception();
    if (str[start] == '}') {
        start++;
        return j;
    }
        
    throw std::exception();
}

json json::parse_array(const std::string& str, std::string::size_type& start)
{
    json j;
    j.type_ = Array;
    start++;
    skip_whilespace(str, start);
    if (start >= str.length())
        throw std::exception();
    if (str[start] == ']') {
        start++;
        return j;
    }
    while (true) {
        skip_whilespace(str, start);
        json v = parse_value(str, start);
        skip_whilespace(str, start);
        j.array_.emplace_back(v);
        if (!eat_symbol(str, start, ",")) {
            break;
        }
    }
    if (start >= str.length())
        throw std::exception();
    if (str[start] == ']') {
        start++;
        return j;
    }
        
    throw std::exception();
}

json json::parse_value(const std::string& str, std::string::size_type& start)
{
    skip_whilespace(str, start);
    // 字面量
    if (eat_symbol(str, start, "true")) return json(true);
    if (eat_symbol(str, start, "false")) return json(false);
    if (eat_symbol(str, start, "null")) return json();

    if (start >= str.length()) throw std::exception();

    if (str[start] == '"') return parse_string(str, start);
    if (str[start] == '{') return parse_object(str, start);
    if (str[start] == '[') return parse_array(str, start);

    return parse_number(str, start);
}

std::string json::dump() const
{
    std::string ret;
    switch (type_) {
    case Object: {
        ret += '{';
        int len = object_.size();
        int pos = 0;
        for (auto i: object_) {
            ret += '"' + i.first + "\": ";
            ret += i.second.dump();
            if (pos++ != len - 1)
                ret += ", ";
        }
        ret += '}';
        break;
    } 
    case Array: {
        ret += '[';
        int len = array_.size(), pos = 0;
        for (auto i: array_) {
            ret += i.dump();
            if (pos++ != len - 1)
                ret += ", ";
        }
        ret += ']';
        break;
    }
    case String:
        ret += '"' + string_ + '"';
        break;
    case Number:
        ret += std::to_string(number_);
        break;
    case Bool:
        ret += bool_ ? "true": "false";
        break;
    case Null:
        ret += "null";
        break;
    default:
        break;
    }
    
    return ret;
}

} // namespace liph

#endif // JSON_HPP_
