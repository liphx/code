#ifndef LIPH_ENCODING_BASE64_H_
#define LIPH_ENCODING_BASE64_H_

#include <string>
#include <string_view>

namespace liph {

struct base64 {
    static std::string encode(std::string_view sv);
    static std::string decode(std::string_view sv);
};

}  // namespace liph

#endif  // LIPH_ENCODING_BASE64_H_
