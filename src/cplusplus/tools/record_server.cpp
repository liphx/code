#include <algorithm>
#include <filesystem>
#include <sstream>

#include "gflags/gflags.h"
#include "httplib.h"
#include "liph/sqlite.h"
#include "nlohmann/json.hpp"
#include "spdlog/sinks/basic_file_sink.h"

using json = nlohmann::json;
namespace fs = std::filesystem;
auto logger = spdlog::basic_logger_mt("basic_logger", "std.log");

DEFINE_string(host, "0.0.0.0", "default host");
DEFINE_int32(port, 8000, "default port");
DEFINE_string(db_path, "./", "");

std::vector<liph::Sqlite> db;

void init_db() {
    db.resize(3);
    if (!db[0].open(FLAGS_db_path + "/liph.db") || !db[1].open(FLAGS_db_path + "/jasmine.db") ||
            !db[2].open(FLAGS_db_path + "/library.db")) {
        exit(1);
    }
}

void toJson(const std::vector<std::vector<std::string>>& in, json& j) {
    j = nullptr;
    if (in.empty()) {
        return;
    }
    for (size_t i = 1; i < in.size(); i++) {
        for (size_t k = 0; k < in[i].size(); k++) {
            j[i - 1][in[0][k]] = in[i][k];
        }
    }
}

void MyMovie(const httplib::Request& req, httplib::Response& res) {
    json body, ret;
    std::string user;
    try {
        body = json::parse(req.body);
        user = body.at("user").get<std::string>();
    } catch (...) {
        logger->error("json::parse error");
        return;
    }

    std::string sql = "select * from movie";

    std::vector<std::vector<std::string>> result;
    ret["data"] = nullptr;
    if (user == "liph") {
        result = db[0].query(sql.c_str());
        toJson(result, ret["data"]);
    } else if (user == "jasmine") {
        result = db[1].query(sql.c_str());
        toJson(result, ret["data"]);
    }

    res.set_content(ret.dump(), "application/json");
    res.set_header("Access-Control-Allow-Origin", "*");
}

void MyBook(const httplib::Request& req, httplib::Response& res) {
    json body, ret;
    std::string user;
    try {
        body = json::parse(req.body);
        user = body.at("user").get<std::string>();
    } catch (...) {
        logger->error("json::parse error");
        return;
    }

    std::string sql = "select * from book";

    std::vector<std::vector<std::string>> result;
    ret["data"] = nullptr;
    if (user == "liph") {
        result = db[0].query(sql.c_str());
        toJson(result, ret["data"]);
    } else if (user == "jasmine") {
        result = db[1].query(sql.c_str());
        toJson(result, ret["data"]);
    }

    res.set_content(ret.dump(), "application/json");
    res.set_header("Access-Control-Allow-Origin", "*");
}

void MyLibrary(const httplib::Request&, httplib::Response& res) {
    json ret;
    std::string sql = "select * from library";

    std::vector<std::vector<std::string>> result;
    ret["data"] = nullptr;
    result = db[2].query(sql.c_str());
    toJson(result, ret["data"]);
    res.set_content(ret.dump(), "application/json");
    res.set_header("Access-Control-Allow-Origin", "*");
}

void AddMovie(const httplib::Request& req, httplib::Response& res) {
    json body, ret;
    std::string title, year, rating, douban, imdb, user;
    try {
        body = json::parse(req.body);
        title = body.at("title").get<std::string>();
        year = body.at("year").get<std::string>();
        rating = body.at("rating").get<std::string>();
        douban = body.at("douban").get<std::string>();
        imdb = body.at("imdb").get<std::string>();
        user = body.at("user").get<std::string>();
    } catch (...) {
        logger->error("json::parse error");
        return;
    }

    std::stringstream ss;
    ss << "INSERT INTO movie(title, year, rating, douban_item, imdb_item, "
          "record_date) ";
    ss << "values(\"" << title << "\", " << year << ", " << rating << ", \"" << douban << "\", \"" << imdb
       << "\", datetime('now','localtime'));";

    int db_ret = SQLITE_OK;
    if (user == "liph") {
        db_ret = db[0].execute(ss.str().c_str());
    } else if (user == "jasmine") {
        db_ret = db[1].execute(ss.str().c_str());
    }
    logger->info(ss.str());

    ret["status"] = db_ret == SQLITE_OK;
    res.set_content(ret.dump(), "application/json");
    res.set_header("Access-Control-Allow-Origin", "*");
}

void AddBook(const httplib::Request& req, httplib::Response& res) {
    logger->info("AddBook");
    json body, ret;
    std::string title, rating, isbn, user;
    try {
        logger->info(req.body);
        body = json::parse(req.body);
        title = body.at("title").get<std::string>();
        rating = body.at("rating").get<std::string>();
        isbn = body.at("isbn").get<std::string>();
        user = body.at("user").get<std::string>();
    } catch (...) {
        logger->error("json::parse error");
        return;
    }

    std::stringstream ss;
    ss << "INSERT INTO book(title, rating, isbn, record_date) ";
    ss << "values('" << title << "', " << rating << ", '" << isbn << "', datetime('now','localtime'));";

    int db_ret = SQLITE_OK;
    if (user == "liph") {
        db_ret = db[0].execute(ss.str().c_str());
    } else if (user == "jasmine") {
        db_ret = db[1].execute(ss.str().c_str());
    }
    logger->info(ss.str());

    ret["status"] = db_ret == SQLITE_OK;
    res.set_content(ret.dump(), "application/json");
    res.set_header("Access-Control-Allow-Origin", "*");
}

void AddLibrary(const httplib::Request& req, httplib::Response& res) {
    logger->info("AddLibrary");
    json body, ret;
    std::string title, origin_title, author, publisher, translator, isbn, douban, tag;
    int publish_year;
    try {
        logger->info(req.body);
        body = json::parse(req.body);
        title = body.at("title").get<std::string>();
        origin_title = body.at("origin_title").get<std::string>();
        author = body.at("author").get<std::string>();
        publisher = body.at("publisher").get<std::string>();
        publish_year = body.at("publish_year").get<int>();
        translator = body.at("translator").get<std::string>();
        isbn = body.at("isbn").get<std::string>();
        douban = body.at("douban").get<std::string>();
        tag = body.at("tag").get<std::string>();
    } catch (...) {
        logger->error("json::parse error");
        return;
    }

    std::stringstream ss;
    ss << "INSERT INTO library(title, origin_title, author, publisher, "
          "publish_year, translator, isbn, douban_item, tag) ";
    ss << "values('" << title << "', '" << origin_title << "', '" << author << "', '" << publisher << "', "
       << publish_year << ", '" << translator << "', '" << isbn << "', '" << douban << "', '" << tag << "');";

    int db_ret = db[2].execute(ss.str().c_str());
    logger->info(ss.str());

    ret["status"] = db_ret == SQLITE_OK;
    res.set_content(ret.dump(), "application/json");
    res.set_header("Access-Control-Allow-Origin", "*");
}

int main(int argc, char *argv[]) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    init_db();

    logger->set_level(spdlog::level::info);
    logger->flush_on(spdlog::level::info);

    httplib::Server svr;

    svr.Post("/api/add_movie", AddMovie);
    svr.Post("/api/my_movie", MyMovie);
    svr.Post("/api/add_book", AddBook);
    svr.Post("/api/my_book", MyBook);
    svr.Post("/api/add_library", AddLibrary);
    svr.Post("/api/my_library", MyLibrary);

    svr.listen(FLAGS_host.c_str(), FLAGS_port);
    return 0;
}
