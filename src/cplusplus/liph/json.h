#ifndef LIPH_JSON_H_
#define LIPH_JSON_H_

#include <string>
#include <unordered_map>
#include <vector>

namespace liph {

class json {
public:
    // number: double or int64
    // boolean literal: "true" or "false"
    // null literal: "null"
    enum value_type { object, array, string, number, boolean, null };

    json();  // null
    explicit json(bool b);
    explicit json(double d);
    explicit json(int32_t n);
    explicit json(int64_t n);
    explicit json(std::string str);
    explicit json(const char *str);  // null type if str is nullptr
    explicit json(const std::vector<json>& arr);
    explicit json(const std::unordered_map<std::string, json>& obj);

    json(const json& other);
    json(json&& other);
    json& operator=(const json& other);
    json& operator=(json&& other);
    ~json();

    // for object, array and string, become empty
    void clear();

    // become null
    void reset();

    static json parse(const std::string& str);
    std::string to_string() const;

    bool operator==(const json& other) const;
    bool operator!=(const json& other) const;

    // for object
    json& at(const std::string& key);
    const json& at(const std::string& key) const;

    // for array
    json& at(const std::size_t& pos);
    const json& at(const std::size_t& pos) const;

    bool& bool_ref();
    double& double_ref();
    int64_t& i64_ref();
    std::string& string_ref();
    std::vector<json>& array_ref();
    std::unordered_map<std::string, json>& object_ref();

private:
    value_type type_{null};
    bool is_double_{false};
    union {
        bool bool_;
        double double_;
        int64_t i64_;
        std::string *string_;
        std::vector<json> *array_;
        std::unordered_map<std::string, json> *object_;
    };
};

}  // namespace liph

static_assert(sizeof(liph::json) == 16);  // alignment

#endif  // LIPH_JSON_H_
