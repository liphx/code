#include "liph/net/requests.h"

namespace liph {
namespace requests {

response request(const std::string& method, const std::string& url) {
    std::string str(method);
    std::transform(str.begin(), str.end(), str.begin(), [](char ch) { return std::toupper(ch); });
    if (str == "GET") return get(url);
    if (str == "POST") return post(url);
    throw std::invalid_argument("Unknown method");
}

response get(const std::string& url) {
    std::string str = "GET " + url + " HTTP/1.1";
    return {};
}

}  // namespace requests
}  // namespace liph
