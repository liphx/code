#include "uno/server.h"

#include "glog/logging.h"
#include "liph/json.h"
#include "liph/numeric/random.h"

namespace uno {

using json = liph::json;

void Server::Whoami(const httplib::Request& req, httplib::Response& res) {
    json ret;
    ret["id"] = liph::random::gen_str(8);
    res.set_content(ret.to_string(), "application/json");
}

void Server::Create(const httplib::Request& req, httplib::Response& res) {
    LOG(INFO) << "Server::Create";
    try {
        json ret;
        auto body = json::parse(req.body);
        const auto& name = body.at("name").string_ref();
        int nums = body.at("nums").i64_ref();
        LOG(INFO) << name << ", " << nums;
        res.set_content(ret.to_string(), "application/json");
    } catch (std::exception& e) {
        LOG(ERROR) << e.what();
    }
}

}  // namespace uno
