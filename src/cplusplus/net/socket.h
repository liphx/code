#pragma once

#include <memory>
#include <string>

namespace liph::net {

enum class domain { ipv4, ipv6 };
enum class protocol { tcp, udp };
class descriptor;

class socket {
public:
    socket();
    socket(domain, protocol);
    socket(socket&&);
    socket& operator=(socket&&);
    ~socket();

    bool connect(const char *ip, int port);

    bool ok() const { return _error.empty(); }
    std::string error() const { return _error; }

private:
    std::unique_ptr<descriptor> _descriptor;
    std::string _error;
    domain _domain;
    protocol _protocol;
};

}  // namespace liph::net

