#ifndef LIPH_SQLITE_H_
#define LIPH_SQLITE_H_

#include <sqlite3.h>

#include <iostream>
#include <string>
#include <vector>

namespace liph {

class Sqlite {
public:
    Sqlite() {}

    Sqlite(const char *filename) : db_(nullptr) {
        int ret = sqlite3_open(filename, &db_);
        if (ret != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(db_));
        }
    }

    Sqlite(const std::string& filename) : Sqlite(filename.c_str()) {}

    bool open(const char *filename) {
        if (db_) return false;
        return sqlite3_open(filename, &db_) == SQLITE_OK;
    }

    bool open(const std::string& filename) { return open(filename.c_str()); }

    /* Sqlite(const Sqlite&) = delete; */

    ~Sqlite() {
        sqlite3_close(db_);
        db_ = nullptr;
    }

    int execute(const char *sql) { return sqlite3_exec(db_, sql, nullptr, nullptr, nullptr); }

    // the first column returns the field name if the result is not empty
    std::vector<std::vector<std::string>> query(const char *sql) {
        std::vector<std::vector<std::string>> result;
        int ret = sqlite3_exec(
                db_, sql,
                [](void *data, int num, char **fields, char **names) {
                    auto *result = (std::vector<std::vector<std::string>> *)data;
                    if (result->empty()) {
                        std::vector<std::string> name;
                        for (int i = 0; i < num; i++) {
                            if (names[i] != nullptr) {
                                name.emplace_back(names[i]);
                            } else {
                                name.emplace_back("");
                            }
                        }
                        result->emplace_back(name);
                    }
                    std::vector<std::string> field;
                    for (int i = 0; i < num; i++) {
                        if (fields[i] != nullptr) {
                            field.emplace_back(fields[i]);
                        } else {
                            field.emplace_back("");
                        }
                    }
                    result->emplace_back(field);
                    return 0;
                },
                &result, nullptr);

        if (ret != SQLITE_OK) {
            std::cerr << sqlite3_errmsg(db_) << std::endl;
        }
        return result;
    }

    sqlite3 *db() const { return db_; }

private:
    sqlite3 *db_{nullptr};
};

class PreparedStatement {
public:
    PreparedStatement(Sqlite& db, const char *sql) : st(nullptr) {
        int ret = sqlite3_prepare_v2(db.db(), sql, -1, &st, nullptr);
        if (ret != SQLITE_OK) {
            throw std::runtime_error(sqlite3_errmsg(db.db()));
        }
    }

    ~PreparedStatement() {
        sqlite3_finalize(st);
        st = nullptr;
    }

    int reset() { return sqlite3_reset(st); }
    int step() { return sqlite3_step(st); }
    int column_int(int index) { return sqlite3_column_int(st, index); }
    double column_double(int index) { return sqlite3_column_double(st, index); }
    std::string column_text(int index) { return (char *)sqlite3_column_text(st, index); }

private:
    sqlite3_stmt *st;
};

}  // namespace liph

#endif  // LIPH_SQLITE_H_
