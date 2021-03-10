#include "../http.h"
using namespace http;

int main(int argc, char *argv[])
{
    http_server server(8080);
    server.handle("/hello", [](auto req, auto res){
        if (req.method() == "get") {
            res.set_content("hello, world.", "text/plain");
        }
    });
    server.start();
}
