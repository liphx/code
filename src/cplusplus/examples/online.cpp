/* // g++ online.cpp -I ~/usr/include -L ~/usr/lib -lsqlite3 -lhttplib -lgflags -lcryptopp -lliph */
/* #include <unistd.h> */

/* #include <algorithm> */
/* #include <cstdlib> */
/* #include <ctime> */
/* #include <fstream> */
/* #include <iostream> */
/* #include <iterator> */
/* #include <map> */

/* #include "gflags/gflags.h" */
/* #include "httplib/httplib.h" */
/* #include "liph/liph.h" */
/* #include "nlohmann/json.hpp" */
/* #include "spdlog/sinks/basic_file_sink.h" */

/* using namespace std; */
/* using namespace liph::crypto; */
/* using json = nlohmann::json; */

/* DEFINE_string(host, "0.0.0.0", "default host"); */
/* DEFINE_int32(port, 9002, "default port"); */
/* DEFINE_string(saved_files, "./saved_files", "default path for saved files"); */
/* DEFINE_string(db_path, "./test.db", "default path for db"); */
/* DEFINE_bool(test, false, "for test"); */

/* auto Logger = spdlog::basic_logger_mt("basic_logger", "std.log"); */
/* liph::Sqlite db(FLAGS_db_path); */
/* string test_session_id = "123456"; */

/* enum { GET_MESSAGE_ALL = 0, GET_MESSAGE_NEW }; */

/* int g_request_count = 0; */

/* bool is_user_exist(string name) { */
/*     if (name == "") { */
/*         return false; */
/*     } */
/*     string sql = "select * from user where name = '" + name + "'"; */
/*     auto result = db.query(sql.c_str()); */
/*     if (result.empty()) { */
/*         return false; */
/*     } */
/*     return true; */
/* } */

/* struct session_info { */
/*     string session_id; */
/*     time_t create_time; */
/* }; */

/* map<string, session_info> session; */

/* void clear_session(void *args) { */
/*     int session_timeout = 20 * 60;  // 20 mins */
/*     for (auto it = session.begin(); it != session.end();) { */
/*         string name = it->first; */
/*         session_info info = it->second; */
/*         time_t now = time(nullptr); */
/*         if (now - info.create_time >= session_timeout) { */
/*             Logger->info("{} timeout", name); */
/*             session.erase(it++); */
/*         } else { */
/*             it++; */
/*         } */
/*     } */
/* } */

/* void clear_file(void *args) { */
/*     // 文件设置7天过期 */
/*     string cmd = string("find ") + FLAGS_saved_files + "/1* -mtime +7 -exec rm -vrf {} \\;"; */
/*     Logger->info(cmd); */
/*     system(cmd.c_str()); */
/* } */

/* struct message_info { */
/*     string from; */
/*     string to; */
/*     string message; */
/*     time_t send_time; */
/* }; */

/* struct file_info { */
/*     string from; */
/*     string to; */
/*     string url; */
/*     time_t send_time; */
/* }; */

/* multimap<string, message_info> message_cache; */
/* multimap<string, file_info> file_info_cache; */
/* multimap<string, string> friend_request; */

/* void print_message_cache() { */
/*     for (auto i : message_cache) { */
/*         Logger->info("print_message_cache {}, {}, {}, {}", i.first, i.second.from, i.second.to, i.second.message); */
/*     } */
/* } */

/* bool check_session(string name, string session_id) { */
/*     if (name == "" || session_id == "") { */
/*         return false; */
/*     } */
/*     auto iter = session.find(name); */
/*     if (iter == session.end() || session_id != iter->second.session_id) { */
/*         return false; */
/*     } */
/*     // update session time */
/*     session[name].create_time = time(nullptr); */

/*     return true; */
/* } */

/* void Information(const httplib::Request& req, httplib::Response& res) { */
/*     Logger->info("POST Information"); */
/*     json body, ret; */
/*     ret["information"] = nullptr; */

/*     string name, session_id; */
/*     try { */
/*         body = json::parse(req.body); */
/*         name = body.at("name").get<string>(); */
/*         session_id = body.at("session_id").get<string>(); */
/*     } catch (exception) { */
/*         Logger->error("json::parse error"); */
/*         res.set_content(ret.dump(), "application/json"); */
/*         return; */
/*     } */

/*     if (check_session(name, session_id)) { */
/*         string sql = "select name, email from user where name = '" + name + "'"; */
/*         Logger->info("sql: {}", sql); */
/*         auto result = db.query(sql.c_str()); */
/*         if (result.size() > 1) { */
/*             for (int i = 0; i < result[0].size(); i++) { */
/*                 ret["information"][result[0][i]] = result[1][i]; */
/*             } */
/*         } */
/*     } */
/*     res.set_content(ret.dump(), "application/json"); */
/* } */

/* void Islogin(const httplib::Request& req, httplib::Response& res) { */
/*     Logger->info("POST Islogin"); */
/*     json body, ret; */
/*     string name, session_id; */
/*     try { */
/*         body = json::parse(req.body); */
/*         name = body.at("name").get<string>(); */
/*         session_id = body.at("session_id").get<string>(); */
/*         ret["status"] = check_session(name, session_id); */
/*     } catch (exception) { */
/*         Logger->error("json::parse error"); */
/*         ret["status"] = false; */
/*     } */

/*     res.set_content(ret.dump(), "application/json"); */
/* } */

/* void Login(const httplib::Request& req, httplib::Response& res) { */
/*     json ret; */
/*     goto start; */
/* err: */
/*     ret["status"] = false; */
/*     ret["session_id"] = nullptr; */
/*     res.set_content(ret.dump(), "application/json"); */
/*     return; */
/* start: */
/*     Logger->info("POST Login"); */
/*     json body; */
/*     string name, passwd; */
/*     try { */
/*         body = json::parse(req.body); */
/*         name = body.at("name").get<string>(); */
/*         passwd = body.at("passwd").get<string>(); */
/*     } catch (exception) { */
/*         Logger->error("json::parse error"); */
/*         goto err; */
/*     } */

/*     if (name == "" || passwd == "") { */
/*         goto err; */
/*     } */

/*     string sql = "select * from user where name = '" + name + "' and passwd = '" + passwd + "'"; */
/*     auto result = db.query(sql.c_str()); */
/*     if (result.empty()) { */
/*         goto err; */
/*     } else { */
/*         ret["status"] = true; */
/*     } */

/*     if (ret["status"]) {  // 记录session 返回给客户端 */
/*         session_info info; */
/*         info.create_time = time(nullptr); */
/*         info.session_id = md5(passwd); */
/*         if (FLAGS_test) {  // for test */
/*             info.session_id = test_session_id; */
/*         } */
/*         session[name] = info; */
/*         ret["session_id"] = info.session_id; */
/*     } */

/*     res.set_content(ret.dump(), "application/json"); */
/* } */

/* void Register(const httplib::Request& req, httplib::Response& res) { */
/*     json ret; */
/*     goto start; */
/* err: */
/*     ret["status"] = false; */
/*     res.set_content(ret.dump(), "application/json"); */
/*     return; */
/* start: */
/*     Logger->info("POST Register"); */
/*     json body; */
/*     string name, passwd, email; */
/*     try { */
/*         body = json::parse(req.body); */
/*         name = body.at("name").get<string>(); */
/*         passwd = body.at("passwd").get<string>(); */
/*         email = body.at("email").get<string>(); */
/*     } catch (exception) { */
/*         Logger->error("json::parse error"); */
/*         goto err; */
/*     } */
/*     if (name == "" || passwd == "" || email == "") { */
/*         goto err; */
/*     } */

/*     string sql = "select * from user where name = '" + name + "'"; */
/*     auto result = db.query(sql.c_str()); */
/*     if (!result.empty()) {  // 用户已存在 */
/*         goto err; */
/*     } */

/*     sql = "insert into user values('" + name + "', '" + passwd + "',  '" + email + "')"; */
/*     int db_ret = db.execute(sql.c_str()); */
/*     if (db_ret != SQLITE_OK) { */
/*         goto err; */
/*     } */

/*     ret["status"] = true; */
/*     res.set_content(ret.dump(), "application/json"); */
/* } */

/* void AlterPassword(const httplib::Request& req, httplib::Response& res) { */
/*     Logger->info("POST AlterPassword"); */
/*     json body, ret; */
/*     ret["status"] = false; */
/*     string name, session_id, old_passwd, new_passwd; */
/*     try { */
/*         body = json::parse(req.body); */
/*         name = body.at("name").get<string>(); */
/*         session_id = body.at("session_id").get<string>(); */
/*         old_passwd = body.at("old_passwd").get<string>(); */
/*         new_passwd = body.at("new_passwd").get<string>(); */
/*     } catch (exception) { */
/*         Logger->error("json::parse error"); */
/*         goto result; */
/*     } */

/*     if (old_passwd != "" && new_passwd != "" && check_session(name, session_id)) { */
/*         string sql = "select * from user where name = '" + name + "' and passwd = '" + old_passwd + "'"; */
/*         auto result = db.query(sql.c_str()); */
/*         if (!result.empty()) { */
/*             sql = "update user set passwd = '" + new_passwd + "' where name = '" + name + "' and passwd = '" + */
/*                   old_passwd + "'"; */
/*             int db_ret = db.execute(sql.c_str()); */
/*             if (db_ret != SQLITE_OK) { */
/*                 ret["status"] = false; */
/*             } else { */
/*                 ret["status"] = true; */
/*             } */
/*         } */
/*     } */
/* result: */
/*     res.set_content(ret.dump(), "application/json"); */
/* } */

/* void AlterInformation(const httplib::Request& req, httplib::Response& res) { */
/*     Logger->info("POST AlterInformation"); */
/*     json body, ret; */
/*     ret["status"] = false; */

/*     string name, session_id, email; */
/*     try { */
/*         body = json::parse(req.body); */
/*         name = body.at("name").get<string>(); */
/*         session_id = body.at("session_id").get<string>(); */
/*         email = body.at("email").get<string>(); */
/*     } catch (exception) { */
/*         Logger->error("json::parse error"); */
/*         goto result; */
/*     } */

/*     if (email != "" && check_session(name, session_id)) { */
/*         string sql = "update user set email = '" + email + "' where name = '" + name + "'"; */
/*         int db_ret = db.execute(sql.c_str()); */
/*         if (db_ret != SQLITE_OK) { */
/*             ret["status"] = false; */
/*         } else { */
/*             ret["status"] = true; */
/*         } */
/*     } */

/* result: */
/*     res.set_content(ret.dump(), "application/json"); */
/* } */

/* void GetFriends(const httplib::Request& req, httplib::Response& res) { */
/*     Logger->info("POST GetFriends"); */
/*     json body, ret; */
/*     ret["friends"] = nullptr; */

/*     string name, session_id; */
/*     try { */
/*         body = json::parse(req.body); */
/*         name = body.at("name").get<string>(); */
/*         session_id = body.at("session_id").get<string>(); */
/*     } catch (exception) { */
/*         Logger->error("json::parse error"); */
/*         goto result; */
/*     } */

/*     if (check_session(name, session_id)) { */
/*         string sql = "select name2 from friends where name1 = '" + name + "'"; */
/*         auto result = db.query(sql.c_str()); */
/*         for (int i = 1; i < result.size(); i++) { */
/*             ret["friends"][i - 1] = result[i][0]; */
/*         } */
/*     } */

/* result: */
/*     res.set_content(ret.dump(), "application/json"); */
/* } */

/* void GetMessage(const httplib::Request& req, httplib::Response& res) { */
/*     Logger->info("POST GetMessage"); */
/*     json body, ret; */
/*     ret["message"] = nullptr; */

/*     string name, session_id, friend_name; */
/*     int how = -1; */
/*     try { */
/*         body = json::parse(req.body); */
/*         name = body.at("name").get<string>(); */
/*         session_id = body.at("session_id").get<string>(); */
/*         friend_name = body.at("friend_name").get<string>(); */
/*         how = body.at("how").get<int>(); */
/*     } catch (exception) { */
/*         Logger->error("json::parse error"); */
/*         goto result; */
/*     } */

/*     if (friend_name != "" && check_session(name, session_id) && how != -1) { */
/*         int index = 0; */
/*         if (how == GET_MESSAGE_ALL) { */
/*             string sql = "select * from message where (name1='" + name + "' and name2='" + friend_name + */
/*                          "') or (name1='" + friend_name + "' and name2='" + name + "')"; */
/*             auto result = db.query(sql.c_str()); */
/*             for (int i = 1; i < result.size(); i++) { */
/*                 ret["message"][index]["name1"] = result[i][0]; */
/*                 ret["message"][index]["name2"] = result[i][1]; */
/*                 ret["message"][index]["message"] = result[i][2]; */
/*                 ret["message"][index]["time"] = std::stoi(result[i][3]); */
/*                 ret["message"][index]["type"] = "message"; */
/*                 index++; */
/*             } */
/*             // clear message cache */
/*             message_cache.erase(name); */

/*             sql = "select * from files where (name1='" + name + "' and name2='" + friend_name + "') or (name1='" + */
/*                   friend_name + "' and name2='" + name + "')"; */
/*             result = db.query(sql.c_str()); */
/*             for (int i = 1; i < result.size(); i++) { */
/*                 ret["message"][index]["name1"] = result[i][0]; */
/*                 ret["message"][index]["name2"] = result[i][1]; */
/*                 ret["message"][index]["message"] = result[i][2]; */
/*                 ret["message"][index]["time"] = std::stoi(result[i][3]); */
/*                 ret["message"][index]["type"] = "file"; */
/*                 index++; */
/*             } */
/*             file_info_cache.erase(name); */
/*         } else if (how == GET_MESSAGE_NEW) { */
/*             bool has_new_message = false; */
/*             int wait = 0; */
/*             for (;;) {  // 长轮询 */

/*                 auto pr = message_cache.equal_range(name); */
/*                 if (pr.first != message_cache.end()) { */
/*                     auto iter = pr.first; */
/*                     int i = 0; */
/*                     for (; iter != pr.second;) { */
/*                         if (friend_name == iter->second.from) { */
/*                             has_new_message = true; */
/*                             ret["message"][index]["name1"] = iter->second.from; */
/*                             ret["message"][index]["name2"] = iter->second.to; */
/*                             ret["message"][index]["message"] = iter->second.message; */
/*                             ret["message"][index]["time"] = iter->second.send_time; */
/*                             ret["message"][index]["type"] = "message"; */
/*                             iter = message_cache.erase(iter);  // 注意迭代器失效 */
/*                             i++; */
/*                             index++; */
/*                         } else { */
/*                             iter++; */
/*                         } */
/*                     } */
/*                 } */

/*                 // saved file, same with message */
/*                 auto pr2 = file_info_cache.equal_range(name); */
/*                 if (pr2.first != file_info_cache.end()) { */
/*                     auto iter = pr2.first; */
/*                     int i = 0; */
/*                     for (; iter != pr2.second;) { */
/*                         if (friend_name == iter->second.from) { */
/*                             has_new_message = true; */
/*                             ret["message"][index]["name1"] = iter->second.from; */
/*                             ret["message"][index]["name2"] = iter->second.to; */
/*                             ret["message"][index]["message"] = iter->second.url; */
/*                             ret["message"][index]["time"] = iter->second.send_time; */
/*                             ret["message"][index]["type"] = "file"; */
/*                             iter = file_info_cache.erase(iter); */
/*                             i++; */
/*                             index++; */
/*                         } else { */
/*                             iter++; */
/*                         } */
/*                     } */
/*                 } */

/*                 if (has_new_message) { */
/*                     break; */
/*                 } else {  // 没有消息，阻塞 */
/*                     wait += 2; */
/*                     // print("sleep 2, wait =", wait); */
/*                     sleep(2); */
/*                 } */

/*                 if (wait >= 30) {  // 超时返回 */
/*                     break; */
/*                 } */
/*             } */
/*         } else { */
/*             Logger->info("how = {}", how); */
/*         } */
/*     } */

/* result: */
/*     print_message_cache(); */
/*     res.set_content(ret.dump(), "application/json"); */
/* } */

/* void SendMessage(const httplib::Request& req, httplib::Response& res) { */
/*     Logger->info("POST SendMessage"); */
/*     json body, ret; */
/*     ret["status"] = false; */

/*     string name, session_id, friend_name, message; */
/*     try { */
/*         body = json::parse(req.body); */
/*         name = body.at("name").get<string>(); */
/*         session_id = body.at("session_id").get<string>(); */
/*         friend_name = body.at("friend_name").get<string>(); */
/*         message = body.at("message").get<string>(); */
/*     } catch (exception) { */
/*         Logger->error("json::parse error"); */
/*         goto result; */
/*     } */

/*     if (!is_user_exist(friend_name)) { */
/*         goto result; */
/*     } */

/*     if (message != "" && check_session(name, session_id)) { */
/*         time_t now = time(nullptr); */
/*         if (FLAGS_test) {  // for test */
/*             now = 0; */
/*         } */

/*         string sql = "insert into message values('" + name + "', '" + friend_name + "', '" + message + "', " + */
/*                      to_string(now) + ")"; */
/*         int db_ret = db.execute(sql.c_str()); */
/*         if (db_ret != SQLITE_OK) { */
/*             ret["status"] = false; */
/*         } else { */
/*             ret["status"] = true; */
/*             message_info info; */
/*             info.from = name; */
/*             info.to = friend_name; */
/*             info.message = message; */
/*             info.send_time = now; */
/*             message_cache.insert(make_pair(friend_name, info)); */
/*         } */
/*     } */

/* result: */
/*     print_message_cache(); */
/*     res.set_content(ret.dump(), "application/json"); */
/* } */

/* void AddFriend(const httplib::Request& req, httplib::Response& res) { */
/*     Logger->info("POST AddFriend"); */
/*     json body, ret; */
/*     ret["status"] = false; */

/*     string name, session_id, friend_name; */
/*     try { */
/*         body = json::parse(req.body); */
/*         name = body.at("name").get<string>(); */
/*         session_id = body.at("session_id").get<string>(); */
/*         friend_name = body.at("friend_name").get<string>(); */
/*     } catch (exception) { */
/*         Logger->error("json::parse error"); */
/*         goto result; */
/*     } */

/*     if (friend_name != "" && check_session(name, session_id)) { */
/*         string sql = "select * from user where name = '" + friend_name + "'"; */
/*         auto result = db.query(sql.c_str()); */
/*         if (result.empty()) {  // 用户不存在 */
/*             ret["status"] = false; */
/*         } else { */
/*             sql = "select * from friends where (name1='" + name + "' and name2='" + friend_name + "')"; */
/*             result = db.query(sql.c_str()); */
/*             if (!result.empty()) {  // 已经是好友 */
/*                 ret["status"] = false; */
/*             } else { */
/*                 ret["status"] = true; */
/*                 bool is_in_request = false; */
/*                 auto pr = friend_request.equal_range(friend_name); */
/*                 if (pr.first != friend_request.end()) { */
/*                     for (auto iter = pr.first; iter != pr.second; iter++) { */
/*                         if (iter->second == name) { */
/*                             is_in_request = true;  // 已经请求过了 */
/*                             break; */
/*                         } */
/*                     } */
/*                 } */
/*                 if (!is_in_request) { */
/*                     friend_request.insert(make_pair(friend_name, name)); */
/*                 } */
/*             } */
/*         } */
/*     } */

/* result: */
/*     // for (auto i: friend_request) { */
/*     //     print("friend_request", i.first, i.second); */
/*     // } */
/*     res.set_content(ret.dump(), "application/json"); */
/* } */

/* void ApplyFriend(const httplib::Request& req, httplib::Response& res) { */
/*     Logger->info("POST ApplyFriend"); */
/*     json body, ret; */
/*     ret["request"] = nullptr; */

/*     string name, session_id; */
/*     try { */
/*         body = json::parse(req.body); */
/*         name = body.at("name").get<string>(); */
/*         session_id = body.at("session_id").get<string>(); */
/*     } catch (exception) { */
/*         Logger->error("json::parse error"); */
/*         goto result; */
/*     } */

/*     if (check_session(name, session_id)) { */
/*         auto pr = friend_request.equal_range(name); */
/*         if (pr.first != friend_request.end()) { */
/*             int i = 0; */
/*             for (auto iter = pr.first; iter != pr.second; iter++, i++) { */
/*                 ret["request"][i] = iter->second; */
/*             } */
/*         } */
/*     } */

/* result: */
/*     res.set_content(ret.dump(), "application/json"); */
/* } */

/* void DealFriend(const httplib::Request& req, httplib::Response& res) { */
/*     Logger->info("POST DealFriend"); */
/*     json body, ret; */
/*     ret["status"] = false; */

/*     string name, session_id, friend_name; */
/*     bool agree; */
/*     try { */
/*         body = json::parse(req.body); */
/*         name = body.at("name").get<string>(); */
/*         session_id = body.at("session_id").get<string>(); */
/*         friend_name = body.at("friend_name").get<string>(); */
/*         agree = body.at("agree").get<bool>(); */
/*     } catch (exception) { */
/*         Logger->error("json::parse error"); */
/*         goto result; */
/*     } */

/*     if (friend_name != "" && check_session(name, session_id)) { */
/*         auto pr = friend_request.equal_range(name); */
/*         if (pr.first != friend_request.end()) { */
/*             for (auto iter = pr.first; iter != pr.second; iter++) { */
/*                 if (iter->second == friend_name) { */
/*                     friend_request.erase(iter); */
/*                     if (agree) {  // 写入db */
/*                         string sql = "insert into friends values('" + name + "', '" + friend_name + "')"; */
/*                         int db_ret = db.execute(sql.c_str()); */
/*                         sql = "insert into friends values('" + friend_name + "', '" + name + "')"; */
/*                         db_ret |= db.execute(sql.c_str()); */
/*                         if (db_ret != SQLITE_OK) { */
/*                             ret["status"] = false; */
/*                         } else { */
/*                             ret["status"] = true; */
/*                         } */
/*                     } */
/*                     break; */
/*                 } */
/*             } */
/*         } */
/*     } */

/* result: */
/*     res.set_content(ret.dump(), "application/json"); */
/* } */

/* void UploadFile(const httplib::Request& req, httplib::Response& res) { */
/*     // form-data */
/*     Logger->info("POST UploadFile"); */
/*     json ret; */
/*     ret["status"] = false; */

/*     string name, session_id, friend_name; */
/*     try { */
/*         name = req.get_file_value("name").content; */
/*         session_id = req.get_file_value("session_id").content; */
/*         friend_name = req.get_file_value("friend_name").content; */
/*     } catch (exception) { */
/*         goto result; */
/*     } */

/*     if (friend_name != "" && check_session(name, session_id)) { */
/*         try { */
/*             time_t now = time(nullptr); */
/*             auto file = req.get_file_value("files"); */
/*             string saved_path = FLAGS_saved_files + string("/") + name + "/" + to_string(now); */
/*             system(("mkdir -p " + saved_path).c_str()); */
/*             ofstream saved_file(saved_path + "/" + file.filename); */
/*             saved_file << file.content; */
/*             saved_file.close(); */

/*             string url = "/saved_files/" + name + "/" + to_string(now) + "/" + file.filename; */
/*             string sql = "insert into files values('" + name + "', '" + friend_name + "', '" + url + "', " + */
/*                          to_string(now) + ")"; */
/*             int db_ret = db.execute(sql.c_str()); */
/*             if (db_ret != SQLITE_OK) { */
/*                 goto result; */
/*             } */

/*             file_info info; */
/*             info.from = name; */
/*             info.to = friend_name; */
/*             info.url = url; */
/*             info.send_time = now; */
/*             file_info_cache.insert(make_pair(friend_name, info)); */
/*             ret["url"] = url; */
/*         } catch (exception) { */
/*             goto result; */
/*         } */
/*         ret["status"] = true; */
/*     } */

/*     Logger->info("{} {} {}", name, session_id, friend_name); */

/* result: */
/*     Logger->info(ret.dump()); */
/*     res.set_content(ret.dump(), "application/json"); */
/* } */

/* int main(int argc, char *argv[]) { */
/*     google::ParseCommandLineFlags(&argc, &argv, true); */

/*     Logger->set_level(spdlog::level::info); */
/*     Logger->flush_on(spdlog::level::info); */

/*     system(("mkdir -p " + FLAGS_saved_files).c_str()); */

/*     liph::Timer timer; */
/*     int clear_session_interval = 10 * 60;    // 10 mins */
/*     int clear_file_interval = 24 * 60 * 60;  // 1 day */
/*     timer.AddJob(clear_session, clear_session_interval, nullptr); */
/*     timer.AddJob(clear_file, clear_file_interval, nullptr); */
/*     pthread_t timer_tid = timer.start(); */

/*     httplib::Server svr; */

/*     svr.Post("/api/register", Register); */
/*     svr.Post("/api/login", Login); */
/*     svr.Post("/api/islogin", Islogin); */
/*     svr.Post("/api/information", Information); */
/*     svr.Post("/api/alter_password", AlterPassword); */
/*     svr.Post("/api/alter_information", AlterInformation); */
/*     svr.Post("/api/get_friends", GetFriends); */
/*     svr.Post("/api/get_message", GetMessage); */
/*     svr.Post("/api/send_message", SendMessage); */
/*     svr.Post("/api/add_friend", AddFriend); */
/*     svr.Post("/api/apply_friend", ApplyFriend); */
/*     svr.Post("/api/deal_friend", DealFriend); */
/*     svr.Post("/api/upload_file", UploadFile); */

/*     svr.listen(FLAGS_host.c_str(), FLAGS_port); */
/*     return 0; */
/* } */

int main() {}
