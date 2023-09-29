#ifndef UNO_SERVER_H_
#define UNO_SERVER_H_

#include "httplib-0.14.0/httplib.h"

namespace uno {

class Server {
public:
    void Start(const std::string& host, int port) {
        svr.Get("/api/whoami", Whoami);
        svr.Post("/api/create", Create);
        svr.listen(host, port);
    }

private:
    static void Whoami(const httplib::Request& req, httplib::Response& res);
    static void Create(const httplib::Request& req, httplib::Response& res);

private:
    httplib::Server svr;
};

}  // namespace uno

#endif  // UNO_SERVER_H_
