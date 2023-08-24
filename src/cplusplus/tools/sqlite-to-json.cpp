#include <cstdlib>

#include "liph/liph.h"
#include "nlohmann/json.hpp"

int main(int argc, char **argv) {
    liph::flags flags;
    flags.register_string_flag("db", std::string(getenv("HOME")) + "/github/liphx/note/record/liph.db");
    flags.register_string_flag("table", "book");
    flags.register_string_flag("sql");
    flags.register_string_flag(
            "out", std::string(getenv("HOME")) + "/github/liphx/note/json/liph-book-sqlite-record.json");
    assert(flags.parse_flags(argc, &argv));
    std::string& flag_db = flags.string_ref("db");
    std::string& flag_table = flags.string_ref("table");
    std::string& flag_sql = flags.string_ref("sql");
    std::string& flag_out = flags.string_ref("out");

    if (flag_db.empty() || (flag_table.empty() && flag_sql.empty())) {
        std::cerr << "Usage: " << argv[0] << " -help" << std::endl;
        liph::err_exit();
    }
    std::string sql = flag_sql;
    if (sql.empty()) {
        sql = "select * from " + flag_table;
    }
    liph::sqlite db(flag_db);
    auto ans = db.query(sql);
    if (ans.empty()) {
        liph::err_exit();
    }
    using json = nlohmann::json;
    auto res = json::array();
    for (int i = 1; i < ans.size(); i++) {
        json item;
        for (int j = 0; j < ans[0].size(); j++) {
            item[ans[0][j]] = ans[i][j];
        }
        res.push_back(item);
    }
    if (flag_out.empty()) {
        liph::print(res.dump(4));  // pass in the amount of spaces to indent
    } else {
        liph::write_file(flag_out, res.dump(4));
    }
}
