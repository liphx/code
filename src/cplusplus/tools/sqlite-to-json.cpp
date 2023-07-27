#include <cstdlib>

#include "gflags/gflags.h"
#include "liph/liph.h"
#include "nlohmann/json.hpp"

DEFINE_string(db, std::string(getenv("HOME")) + "/github/liphx/note/record/liph.db", "");
DEFINE_string(table, "book", "");
DEFINE_string(sql, "", "");
DEFINE_string(out, std::string(getenv("HOME")) + "/github/liphx/note/json/liph-book-sqlite-record.json", "");

int main(int argc, char **argv) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    if (FLAGS_db.empty() || (FLAGS_table.empty() && FLAGS_sql.empty())) {
        std::cerr << "Usage: " << argv[0] << " -help" << std::endl;
        liph::err_exit();
    }
    std::string sql = FLAGS_sql;
    if (sql.empty()) {
        sql = "select * from " + FLAGS_table;
    }
    liph::Sqlite db(FLAGS_db);
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
    if (FLAGS_out.empty()) {
        liph::print(res.dump(4));  // pass in the amount of spaces to indent
    } else {
        liph::write_file(FLAGS_out, res.dump(4));
    }
}
