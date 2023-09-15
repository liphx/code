#include "online/server.h"

#include <unistd.h>

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>

#include "liph/liph.h"
#include "online/user.h"

using namespace std;
using namespace liph;
using namespace liph::crypto;

namespace online {

/* User user(flags.string_ref("db_path")); */
/* liph::sqlite db(flags.string_ref("db_path")); */
User user("/tmp/user.db");
liph::sqlite db("/tmp/user.db");

static const std::string test_session_id = "123456";

enum { GET_MESSAGE_ALL = 0, GET_MESSAGE_NEW };

struct session_info {
    std::string session_id;
    time_t create_time;
};

map<std::string, session_info> session;

void clear_session(void *args) {
    int session_timeout = 20 * 60;  // 20 mins
    for (auto it = session.begin(); it != session.end();) {
        std::string name = it->first;
        session_info info = it->second;
        time_t now = time(nullptr);
        if (now - info.create_time >= session_timeout) {
            LOG << name << " timeout";
            session.erase(it++);
        } else {
            it++;
        }
    }
}

void clear_file(void *args) {
    // 文件设置7天过期
    const auto& FLAGS_saved_files = liph::singleton<liph::flags>::instance().string_ref("saved_files");
    std::string cmd = std::string("find ") + FLAGS_saved_files + "/* -mtime +7 -exec rm -vrf {} \\;";
    LOG << cmd;
    system(cmd.c_str());
}

struct message_info {
    std::string from;
    std::string to;
    std::string message;
    time_t send_time;
};

struct file_info {
    std::string from;
    std::string to;
    std::string url;
    time_t send_time;
};

multimap<std::string, message_info> message_cache;
multimap<std::string, file_info> file_info_cache;
multimap<std::string, std::string> friend_request;

void print_message_cache() {
    for (auto i : message_cache) {
        LOG << format("print_message_cache {}, {}, {}, {}", i.first, i.second.from, i.second.to, i.second.message);
    }
}

bool check_session(std::string name, std::string session_id) {
    if (name == "" || session_id == "") {
        return false;
    }
    auto iter = session.find(name);
    if (iter == session.end() || session_id != iter->second.session_id) {
        return false;
    }
    // update session time
    session[name].create_time = time(nullptr);

    return true;
}

void Information(const httplib::Request& req, httplib::Response& res) {
    json body, ret;
    ret["information"] = nullptr;

    std::string name, session_id;
    try {
        body = json::parse(req.body);
        name = body.at("name").string_ref();
        session_id = body.at("session_id").string_ref();
    } catch (exception&) {
        res.set_content(ret.to_string(), "application/json");
        return;
    }

    if (check_session(name, session_id)) {
        std::string sql = "select name, email from user where name = '" + name + "'";
        auto result = db.query(sql.c_str());
        if (result.size() > 1) {
            for (int i = 0; i < result[0].size(); i++) {
                ret["information"][result[0][i]] = result[1][i];
            }
        }
    }
    res.set_content(ret.to_string(), "application/json");
}

void Islogin(const httplib::Request& req, httplib::Response& res) {
    json body, ret;
    std::string name, session_id;
    try {
        body = json::parse(req.body);
        name = body.at("name").string_ref();
        session_id = body.at("session_id").string_ref();
        ret["status"] = check_session(name, session_id);
    } catch (exception) {
        ret["status"] = false;
    }

    res.set_content(ret.to_string(), "application/json");
}

void Login(const httplib::Request& req, httplib::Response& res) {
    json ret;
    goto start;
err:
    ret["status"] = false;
    ret["session_id"] = nullptr;
    res.set_content(ret.to_string(), "application/json");
    return;
start:
    json body;
    std::string name, passwd;
    try {
        body = json::parse(req.body);
        name = body.at("name").string_ref();
        passwd = body.at("passwd").string_ref();
    } catch (exception) {
        goto err;
    }

    if (name == "" || passwd == "") {
        goto err;
    }

    std::string sql = "select * from user where name = '" + name + "' and passwd = '" + passwd + "'";
    auto result = db.query(sql.c_str());
    if (result.empty()) {
        goto err;
    } else {
        ret["status"] = true;
    }

    if (ret["status"].bool_ref()) {  // 记录session 返回给客户端
        session_info info;
        info.create_time = time(nullptr);
        info.session_id = md5(passwd);
        if (liph::singleton<liph::flags>::instance().bool_ref("test")) {  // for test
            info.session_id = test_session_id;
        }
        session[name] = info;
        ret["session_id"] = info.session_id;
    }

    res.set_content(ret.to_string(), "application/json");
}

void AlterPassword(const httplib::Request& req, httplib::Response& res) {
    json body, ret;
    ret["status"] = false;
    std::string name, session_id, old_passwd, new_passwd;
    try {
        body = json::parse(req.body);
        name = body.at("name").string_ref();
        session_id = body.at("session_id").string_ref();
        old_passwd = body.at("old_passwd").string_ref();
        new_passwd = body.at("new_passwd").string_ref();
    } catch (exception) {
        goto result;
    }

    if (old_passwd != "" && new_passwd != "" && check_session(name, session_id)) {
        std::string sql = "select * from user where name = '" + name + "' and passwd = '" + old_passwd + "'";
        auto result = db.query(sql.c_str());
        if (!result.empty()) {
            sql = "update user set passwd = '" + new_passwd + "' where name = '" + name + "' and passwd = '" +
                  old_passwd + "'";
            int db_ret = db.execute(sql.c_str());
            if (db_ret != SQLITE_OK) {
                ret["status"] = false;
            } else {
                ret["status"] = true;
            }
        }
    }
result:
    res.set_content(ret.to_string(), "application/json");
}

void AlterInformation(const httplib::Request& req, httplib::Response& res) {
    json body, ret;
    ret["status"] = false;

    std::string name, session_id, email;
    try {
        body = json::parse(req.body);
        name = body.at("name").string_ref();
        session_id = body.at("session_id").string_ref();
        email = body.at("email").string_ref();
    } catch (exception) {
        goto result;
    }

    if (email != "" && check_session(name, session_id)) {
        std::string sql = "update user set email = '" + email + "' where name = '" + name + "'";
        int db_ret = db.execute(sql.c_str());
        if (db_ret != SQLITE_OK) {
            ret["status"] = false;
        } else {
            ret["status"] = true;
        }
    }

result:
    res.set_content(ret.to_string(), "application/json");
}

void GetFriends(const httplib::Request& req, httplib::Response& res) {
    json body, ret;
    ret["friends"] = nullptr;

    std::string name, session_id;
    try {
        body = json::parse(req.body);
        name = body.at("name").string_ref();
        session_id = body.at("session_id").string_ref();
    } catch (exception) {
        goto result;
    }

    if (check_session(name, session_id)) {
        std::string sql = "select name2 from friends where name1 = '" + name + "'";
        auto result = db.query(sql.c_str());
        for (int i = 1; i < result.size(); i++) {
            ret["friends"][i - 1] = result[i][0];
        }
    }

result:
    res.set_content(ret.to_string(), "application/json");
}

void GetMessage(const httplib::Request& req, httplib::Response& res) {
    json body, ret;
    ret["message"] = nullptr;

    std::string name, session_id, friend_name;
    int how = -1;
    try {
        body = json::parse(req.body);
        name = body.at("name").string_ref();
        session_id = body.at("session_id").string_ref();
        friend_name = body.at("friend_name").string_ref();
        how = body.at("how").i64_ref();
    } catch (exception) {
        goto result;
    }

    if (friend_name != "" && check_session(name, session_id) && how != -1) {
        int index = 0;
        if (how == GET_MESSAGE_ALL) {
            std::string sql = "select * from message where (name1='" + name + "' and name2='" + friend_name +
                              "') or (name1='" + friend_name + "' and name2='" + name + "')";
            auto result = db.query(sql.c_str());
            for (int i = 1; i < result.size(); i++) {
                ret["message"][index]["name1"] = result[i][0];
                ret["message"][index]["name2"] = result[i][1];
                ret["message"][index]["message"] = result[i][2];
                ret["message"][index]["time"] = std::stoi(result[i][3]);
                ret["message"][index]["type"] = "message";
                index++;
            }
            // clear message cache
            message_cache.erase(name);

            sql = "select * from files where (name1='" + name + "' and name2='" + friend_name + "') or (name1='" +
                  friend_name + "' and name2='" + name + "')";
            result = db.query(sql.c_str());
            for (int i = 1; i < result.size(); i++) {
                ret["message"][index]["name1"] = result[i][0];
                ret["message"][index]["name2"] = result[i][1];
                ret["message"][index]["message"] = result[i][2];
                ret["message"][index]["time"] = std::stoi(result[i][3]);
                ret["message"][index]["type"] = "file";
                index++;
            }
            file_info_cache.erase(name);
        } else if (how == GET_MESSAGE_NEW) {
            bool has_new_message = false;
            int wait = 0;
            for (;;) {  // 长轮询

                auto pr = message_cache.equal_range(name);
                if (pr.first != message_cache.end()) {
                    auto iter = pr.first;
                    for (; iter != pr.second;) {
                        if (friend_name == iter->second.from) {
                            has_new_message = true;
                            ret["message"][index]["name1"] = iter->second.from;
                            ret["message"][index]["name2"] = iter->second.to;
                            ret["message"][index]["message"] = iter->second.message;
                            ret["message"][index]["time"] = json(static_cast<int64_t>(iter->second.send_time));
                            ret["message"][index]["type"] = "message";
                            iter = message_cache.erase(iter);  // 注意迭代器失效
                            index++;
                        } else {
                            iter++;
                        }
                    }
                }

                // saved file, same with message
                auto pr2 = file_info_cache.equal_range(name);
                if (pr2.first != file_info_cache.end()) {
                    auto iter = pr2.first;
                    for (; iter != pr2.second;) {
                        if (friend_name == iter->second.from) {
                            has_new_message = true;
                            ret["message"][index]["name1"] = iter->second.from;
                            ret["message"][index]["name2"] = iter->second.to;
                            ret["message"][index]["message"] = iter->second.url;
                            ret["message"][index]["time"] = json(static_cast<int64_t>(iter->second.send_time));
                            ret["message"][index]["type"] = "file";
                            iter = file_info_cache.erase(iter);
                            index++;
                        } else {
                            iter++;
                        }
                    }
                }

                if (has_new_message) {
                    break;
                } else {  // 没有消息，阻塞
                    wait += 2;
                    // print("sleep 2, wait =", wait);
                    sleep(2);
                }

                if (wait >= 30) {  // 超时返回
                    break;
                }
            }
        } else {
            LOG << "how = " << how;
        }
    }

result:
    print_message_cache();
    res.set_content(ret.to_string(), "application/json");
}

void SendMessage(const httplib::Request& req, httplib::Response& res) {
    json body, ret;
    ret["status"] = false;

    std::string name, session_id, friend_name, message;
    try {
        body = json::parse(req.body);
        name = body.at("name").string_ref();
        session_id = body.at("session_id").string_ref();
        friend_name = body.at("friend_name").string_ref();
        message = body.at("message").string_ref();
    } catch (exception) {
        goto result;
    }

    if (!user.exists(friend_name)) {
        goto result;
    }

    if (message != "" && check_session(name, session_id)) {
        time_t now = time(nullptr);
        if (liph::singleton<liph::flags>::instance().bool_ref("test")) {  // for test
            now = 0;
        }

        std::string sql = "insert into message values('" + name + "', '" + friend_name + "', '" + message + "', " +
                          to_string(now) + ")";
        int db_ret = db.execute(sql.c_str());
        if (db_ret != SQLITE_OK) {
            ret["status"] = false;
        } else {
            ret["status"] = true;
            message_info info;
            info.from = name;
            info.to = friend_name;
            info.message = message;
            info.send_time = now;
            message_cache.insert(make_pair(friend_name, info));
        }
    }

result:
    print_message_cache();
    res.set_content(ret.to_string(), "application/json");
}

void AddFriend(const httplib::Request& req, httplib::Response& res) {
    json body, ret;
    ret["status"] = false;

    std::string name, session_id, friend_name;
    try {
        body = json::parse(req.body);
        name = body.at("name").string_ref();
        session_id = body.at("session_id").string_ref();
        friend_name = body.at("friend_name").string_ref();
    } catch (exception) {
        goto result;
    }

    if (friend_name != "" && check_session(name, session_id)) {
        std::string sql = "select * from user where name = '" + friend_name + "'";
        auto result = db.query(sql.c_str());
        if (result.empty()) {  // 用户不存在
            ret["status"] = false;
        } else {
            sql = "select * from friends where (name1='" + name + "' and name2='" + friend_name + "')";
            result = db.query(sql.c_str());
            if (!result.empty()) {  // 已经是好友
                ret["status"] = false;
            } else {
                ret["status"] = true;
                bool is_in_request = false;
                auto pr = friend_request.equal_range(friend_name);
                if (pr.first != friend_request.end()) {
                    for (auto iter = pr.first; iter != pr.second; iter++) {
                        if (iter->second == name) {
                            is_in_request = true;  // 已经请求过了
                            break;
                        }
                    }
                }
                if (!is_in_request) {
                    friend_request.insert(make_pair(friend_name, name));
                }
            }
        }
    }

result:
    // for (auto i: friend_request) {
    //     print("friend_request", i.first, i.second);
    // }
    res.set_content(ret.to_string(), "application/json");
}

void ApplyFriend(const httplib::Request& req, httplib::Response& res) {
    json body, ret;
    ret["request"] = nullptr;

    std::string name, session_id;
    try {
        body = json::parse(req.body);
        name = body.at("name").string_ref();
        session_id = body.at("session_id").string_ref();
    } catch (exception) {
        goto result;
    }

    if (check_session(name, session_id)) {
        auto pr = friend_request.equal_range(name);
        if (pr.first != friend_request.end()) {
            int i = 0;
            for (auto iter = pr.first; iter != pr.second; iter++, i++) {
                ret["request"][i] = iter->second;
            }
        }
    }

result:
    res.set_content(ret.to_string(), "application/json");
}

void DealFriend(const httplib::Request& req, httplib::Response& res) {
    json body, ret;
    ret["status"] = false;

    std::string name, session_id, friend_name;
    bool agree;
    try {
        body = json::parse(req.body);
        name = body.at("name").string_ref();
        session_id = body.at("session_id").string_ref();
        friend_name = body.at("friend_name").string_ref();
        agree = body.at("agree").bool_ref();
    } catch (exception) {
        goto result;
    }

    if (friend_name != "" && check_session(name, session_id)) {
        auto pr = friend_request.equal_range(name);
        if (pr.first != friend_request.end()) {
            for (auto iter = pr.first; iter != pr.second; iter++) {
                if (iter->second == friend_name) {
                    friend_request.erase(iter);
                    if (agree) {  // 写入db
                        std::string sql = "insert into friends values('" + name + "', '" + friend_name + "')";
                        int db_ret = db.execute(sql.c_str());
                        sql = "insert into friends values('" + friend_name + "', '" + name + "')";
                        db_ret |= db.execute(sql.c_str());
                        if (db_ret != SQLITE_OK) {
                            ret["status"] = false;
                        } else {
                            ret["status"] = true;
                        }
                    }
                    break;
                }
            }
        }
    }

result:
    res.set_content(ret.to_string(), "application/json");
}

void UploadFile(const httplib::Request& req, httplib::Response& res) {
    // form-data
    json ret;
    ret["status"] = false;

    std::string name, session_id, friend_name;
    try {
        name = req.get_file_value("name").content;
        session_id = req.get_file_value("session_id").content;
        friend_name = req.get_file_value("friend_name").content;
    } catch (exception) {
        goto result;
    }

    if (friend_name != "" && check_session(name, session_id)) {
        try {
            time_t now = time(nullptr);
            auto file = req.get_file_value("files");
            const auto& FLAGS_saved_files = liph::singleton<liph::flags>::instance().string_ref("saved_files");
            std::string saved_path = FLAGS_saved_files + std::string("/") + name + "/" + to_string(now);
            mkdirp(saved_path);
            ofstream saved_file(saved_path + "/" + file.filename);
            saved_file << file.content;
            saved_file.close();

            std::string url = "/saved_files/" + name + "/" + to_string(now) + "/" + file.filename;
            std::string sql = "insert into files values('" + name + "', '" + friend_name + "', '" + url + "', " +
                              to_string(now) + ")";
            int db_ret = db.execute(sql.c_str());
            if (db_ret != SQLITE_OK) {
                goto result;
            }

            file_info info;
            info.from = name;
            info.to = friend_name;
            info.url = url;
            info.send_time = now;
            file_info_cache.insert(make_pair(friend_name, info));
            ret["url"] = url;
        } catch (exception) {
            goto result;
        }
        ret["status"] = true;
    }

    LOG << format("{} {} {}", name, session_id, friend_name);

result:
    res.set_content(ret.to_string(), "application/json");
}

void Server::Register(const httplib::Request& req, httplib::Response& res) {
    liph::json body, ret;
    std::string name, password;
    try {
        body = liph::json::parse(req.body);
        name = body.at("name").string_ref();
        password = body.at("password").string_ref();
    } catch (std::exception&) {
        LOG << "json::parse error";
        ret["status"] = false;
        res.set_content(ret.to_string(), "application/json");
    }
    ret["status"] = user.add(name, password);
    res.set_content(ret.to_string(), "application/json");
}

}  // namespace online
