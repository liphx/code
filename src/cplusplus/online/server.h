#ifndef ONLINE_SERVER_H_
#define ONLINE_SERVER_H_

#include "liph/json.h"
#include "thirdparty/httplib-0.14.0/httplib.h"

namespace online {

class Server {
public:
    void start(const std::string& host, int port) {
        svr.Post("/api/register", Register);
        /* svr.Post("/api/login", Login); */
        /* svr.Post("/api/islogin", Islogin); */
        /* svr.Post("/api/information", Information); */
        /* svr.Post("/api/alter_password", AlterPassword); */
        /* svr.Post("/api/alter_information", AlterInformation); */
        /* svr.Post("/api/get_friends", GetFriends); */
        /* svr.Post("/api/get_message", GetMessage); */
        /* svr.Post("/api/send_message", SendMessage); */
        /* svr.Post("/api/add_friend", AddFriend); */
        /* svr.Post("/api/apply_friend", ApplyFriend); */
        /* svr.Post("/api/deal_friend", DealFriend); */
        /* svr.Post("/api/upload_file", UploadFile); */
        svr.listen(host, port);
    }

private:
    static void Register(const httplib::Request& req, httplib::Response& res);

private:
    httplib::Server svr;
};

}  // namespace online

#endif  // ONLINE_SERVER_H_
