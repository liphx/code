#ifndef LIPH_JSON_H_
#define LIPH_JSON_H_

#include <filesystem>
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

    // no explicit
    json(value_type type = null);
    json(bool b);
    json(double d);
    json(int32_t n);
    json(int64_t n);
    json(std::string str);
    json(const char *str);  // null type if str is nullptr
    json(const std::vector<json>& arr);
    json(const std::unordered_map<std::string, json>& obj);

    json(const json& other);
    json(json&& other);
    json& operator=(const json& other);
    json& operator=(json&& other);
    ~json();

    value_type type() const;

    // for object, array and string, become empty
    void clear();

    // become null
    void reset();

    static json parse(const std::string& str);
    static json load(const std::filesystem::path& path);
    std::string to_string(int indent = 0, bool sort_keys = false) const;

    bool operator==(const json& other) const;
    bool operator!=(const json& other) const;

    // for object
    json& at(const std::string& key);
    const json& at(const std::string& key) const;
    json& operator[](const std::string& key);              // if null, become object
    const json& operator[](const std::string& key) const;  // same as at()

    // for array
    json& at(std::size_t pos);
    const json& at(std::size_t pos) const;
    json& operator[](std::size_t pos);              // if null, become array
    const json& operator[](std::size_t pos) const;  // same as at()

    bool& bool_ref();
    bool bool_ref() const;
    double& double_ref();
    double double_ref() const;
    int64_t& i64_ref();
    int64_t i64_ref() const;
    std::string& string_ref();
    const std::string& string_ref() const;
    std::vector<json>& array_ref();
    const std::vector<json>& array_ref() const;
    std::unordered_map<std::string, json>& object_ref();
    const std::unordered_map<std::string, json>& object_ref() const;

    bool is_null() const;
    bool is_boolean() const;
    bool is_number() const;
    bool is_string() const;
    bool is_array() const;
    bool is_object() const;

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

static_assert(sizeof(int64_t) == sizeof(double));
static_assert(sizeof(liph::json) == 16);  // alignment

#endif  // LIPH_JSON_H_
