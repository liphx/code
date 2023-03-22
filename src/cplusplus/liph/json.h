#ifndef LIPH_JSON_H_
#define LIPH_JSON_H_

#include <string>
#include <unordered_map>
#include <vector>

namespace liph {

class json {
public:
    enum type { Object, Array, String, Number, Bool, Null };

    json();
    explicit json(bool b);
    explicit json(double n);
    explicit json(const std::string& str);
    explicit json(const char *str);
    explicit json(const std::vector<json>& arr);
    explicit json(const std::unordered_map<std::string, json>& obj);

    json(const json& other);
    json& operator=(const json& other);
    ~json();

    void clear();

    static json parse(const std::string& str);
    std::string to_string() const;

    bool operator==(const json& other) const;
    bool operator!=(const json& other) const;

    json& at(const std::string& key);
    const json& at(const std::string& key) const;
    json& at(const std::size_t& pos);
    const json& at(const std::size_t& pos) const;

    bool& get_bool();
    double& get_number();
    std::string& get_string();
    std::vector<json>& get_array();
    std::unordered_map<std::string, json>& get_object();

private:
    type type_;

    union {
        bool bool_;
        double number_;
        std::string *string_;
        std::vector<json> *array_;
        std::unordered_map<std::string, json> *object_;
    };
};

}  // namespace liph

#endif  // LIPH_JSON_H_
