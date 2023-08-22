#include "liph/net/requests.h"

#include <algorithm>
#include <stdexcept>

namespace liph {
namespace requests {

response request(const std::string& method, const std::string& url) {
    std::string str(method);
    std::transform(str.begin(), str.end(), str.begin(), [](char ch) { return std::toupper(ch); });
    if (str == "GET") return get(url);
    if (str == "POST") return post(url);
    throw std::invalid_argument("Unknown method");
}

// ip[:port][/url]
response get(const std::string& url) {
    // GET / HTTP/1.1
    // Host: localhost:8000
    // User-Agent: liph::requests
    // Accept: */*
    std::string str = "GET " + url + " HTTP/1.1";
    return {};
}

response post(const std::string& url) { return {}; }

}  // namespace requests
}  // namespace liph
