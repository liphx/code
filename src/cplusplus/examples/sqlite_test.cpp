#include "sqlite.h"

#include <filesystem>
#include <fstream>
#include <iostream>

using namespace liph;

int main() {
    const char *db_path = "./.test.db";
    Sqlite db(db_path);
    int ret = SQLITE_OK;
    ret |= db.execute("create table IF NOT EXISTS t(aaa string, bbb int, ccc string)");
    ret |= db.execute("insert into t values('kkk', 123, 'hello')");
    ret |= db.execute("insert into t values('liph', -98, 'cc')");
    ret |= db.execute("insert into t(aaa) values('okk')");
    if (ret != SQLITE_OK) {
        return 1;
    }
    auto result = db.query("select * from t");
    for (auto x : result) {
        for (auto y : x) std::cout << y << "\t";
        std::cout << std::endl;
    }

    std::cout << "PreparedStatement" << std::endl;
    PreparedStatement st(db, "select * from t");
    std::cout << "aaa\tbbb\n===\t===" << std::endl;
    while (st.step() == SQLITE_ROW) {
        std::cout << st.column_text(0) << "\t" << st.column_int(1) << std::endl;
    }

    std::filesystem::remove(db_path);
}
