#include "liph/sqlite.h"

#include <filesystem>
#include <fstream>
#include <iostream>

#include "gtest/gtest.h"
#include "liph/logging.h"

using namespace liph;

TEST(sqlite, db) {
    const char *db_path = "./.test.db";
    sqlite db(db_path);
    int ret = SQLITE_OK;
    ret |= db.execute("create table IF NOT EXISTS t(aaa text, bbb int, ccc text)");
    ret |= db.execute("insert into t values('kkk', 123, 'hello')");
    ret |= db.execute("insert into t values('liph', -98, 'cc')");
    ret |= db.execute("insert into t(aaa) values('okk')");
    EXPECT_EQ(ret, SQLITE_OK);
    auto result = db.query("select * from t");
    for (auto x : result) {
        std::string str;
        for (auto y : x) {
            str = str + y + "\t";
        }
        LOG << str;
    }

    LOG << "PreparedStatement";
    prepared_statement st(db, "select * from t");
    LOG << "aaa\tbbb";
    LOG << "===\t===";
    while (st.step() == SQLITE_ROW) {
        LOG << st.column_text(0) << "\t" << st.column_int(1);
    }

    std::filesystem::remove(db_path);
}
