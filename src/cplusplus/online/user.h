#ifndef ONLINE_USER_H_
#define ONLINE_USER_H_

#include "liph/logging.h"
#include "liph/sqlite.h"

namespace online {

class User {
public:
    User(const std::string& db_path) : db(db_path) {}

    bool exists(const std::string& user_name) const {
        if (!check_user_name(user_name)) return false;
        auto sql = liph::format("select * from user where name = '{}'", user_name);
        auto result = db.query(sql);
        return !result.empty();
    }

    bool add(const std::string& user_name, const std::string& password) {
        if (exists(user_name)) return false;
        // todo, check password or sql
        auto sql = liph::format("insert into user values('{}', '{}')", user_name, password);
        LOG << sql;
        return db.execute(sql) == SQLITE_OK;
    }

private:
    static bool check_user_name(const std::string& name) {
        return !name.empty() && std::all_of(name.begin(), name.end(), [](char ch) { return ch == '_' || isalnum(ch); });
    }

private:
    liph::sqlite db;
};

}  // namespace online

#endif  // ONLINE_USER_H_
