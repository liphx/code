#ifndef JSON_HPP_
#define JSON_HPP_

#include <string>
#include <vector>
#include <map>

namespace liph {

class json {
public:
    enum type { Object, Array, String, Number, Bool, Null };
    json(): type_(Null) {}
    json& operator[](const std::string& key);
    static json parse(const std::string& str);
    std::string dump() const;

private:
    type type_;
    std::map<std::string, json> object_;
    std::vector<json> array_;
    std::string string_;
    int number_;
    bool bool_;
};

json json::parse(const std::string& str)
{
    return json();
}

std::string json::dump() const
{
    std::string ret;
    switch (type_) {
    case Object:
        ret += '{';
        for (auto i: object_) {
            ret += '"' + i.first + "\":";
            ret += i.second.dump();
        }
        ret += '}';
        break;
    case Array:
        ret += '[';
        for (auto i: array_) {
            ret += i.dump();
        }
        ret += ']';
        break;
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
