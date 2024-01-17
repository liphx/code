#ifndef LIPH_STRING_H_
#define LIPH_STRING_H_

#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace liph {

/// Remove whitespace characters to the left of s
/// @param charset default: whitespace(" \n\r\t\f\v")
/// @return sub string_view of s
std::string_view ltrim(std::string_view s, std::string_view charset = " \n\r\t\f\v");

/// Remove whitespace characters to the right of s
/// @see ltrim
std::string_view rtrim(std::string_view s, std::string_view charset = " \n\r\t\f\v");

/// Remove whitespace characters to the left and right of s
/// @see ltrim
std::string_view trim(std::string_view s, std::string_view charset = " \n\r\t\f\v");

/// @return true if s starts with t
constexpr bool startswith(std::string_view s, std::string_view t) noexcept { return s.starts_with(t); }

/// @return true if s ends with t
constexpr bool endswith(std::string_view s, std::string_view t) noexcept { return s.ends_with(t); }

/// Replace all @old in @str with @New
std::string replace(std::string_view str, std::string_view old, std::string_view New);

/// Split @s to tokens by @delimiters and append to @tokens
void split(std::vector<std::string>& tokens, const std::string& s, const std::string& delimiters = " ");

/// Split @s to tokens by @delimiters and return
std::vector<std::string> split(const std::string& s, const std::string& delimiters = " ");

/// Return basename of @path
std::string basename(std::string_view path);

/// Return dirname of @path
std::string dirname(std::string_view path);

/// Convert source to target
template <typename Target = std::string, typename Source = std::string>
Target to(const Source& s) {
    std::stringstream buf;
    Target result;
    if (!(buf << s) || !(buf >> result) || !(buf >> std::ws).eof()) throw std::runtime_error{"to<>() failed"};
    return result;
}

/// @sv skips leading whitespace characters and be the substr
void skip_whitespace(std::string_view& sv);

/// @sv skips leading @symbol and be the substr
/// @return true if @sv start swith @symbol
bool eat_symbol(std::string_view& sv, std::string_view symbol);

/// Convert @str to int
std::optional<int> to_int(std::string_view str);

/// Join all items in @range into a string, using @sep as separator
template <std::ranges::input_range Range>
std::string join(const Range& range, const std::string& sep = " ") {
    std::string str;
    bool first = true;
    for (const auto& value : range) {
        if (!first) str += sep;
        first = false;
        if constexpr (std::is_same<std::ranges::range_value_t<Range>, std::string>::value)
            str += value;
        else
            str += std::to_string(value);
    }
    return str;
}

}  // namespace liph

#endif  // LIPH_STRING_H_
