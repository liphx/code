#ifndef LIPH_NET_REQUESTS_H_
#define LIPH_NET_REQUESTS_H_

#include <string>

namespace liph {
namespace requests {

class response {};

response request(const std::string& method, const std::string& url);
response get(const std::string& url);
response post(const std::string& url);

}  // namespace requests
}  // namespace liph

#endif  // LIPH_NET_REQUESTS_H_
