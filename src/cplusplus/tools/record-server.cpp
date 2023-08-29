#include <algorithm>
#include <filesystem>
#include <sstream>

#include "httplib.h"
#include "liph/liph.h"

using json = liph::json;
namespace fs = std::filesystem;

std::string FLAGS_db_path;
int32_t FLAGS_port;
std::string FLAGS_host;

std::vector<liph::sqlite> db;

void init_db() {
    std::vector<std::string> paths{
            FLAGS_db_path + "/liph.db", FLAGS_db_path + "/jasmine.db", FLAGS_db_path + "/library.db"};
    db.resize(paths.size());
    for (auto i = 0u; i < paths.size(); ++i) {
        // fail if not found
        if (!fs::is_regular_file(paths[i]) || !db[i].open(paths[i])) {
            std::cerr << "db not found, path = " << FLAGS_db_path << std::endl;
            exit(1);
        }
    }
}

void to_json(const std::vector<std::vector<std::string>>& in, liph::json& j) {
    j.reset();
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
        user = body.at("user").string_ref();
    } catch (std::exception& e) {
        LOG << e.what();
        return;
    }

    std::string sql = "select * from movie";

    std::vector<std::vector<std::string>> result;
    ret["data"] = nullptr;
    if (user == "liph") {
        result = db[0].query(sql.c_str());
        ::to_json(result, ret["data"]);
    } else if (user == "jasmine") {
        result = db[1].query(sql.c_str());
        ::to_json(result, ret["data"]);
    }

    res.set_content(ret.to_string(), "application/json");
    res.set_header("Access-Control-Allow-Origin", "*");
}

void MyBook(const httplib::Request& req, httplib::Response& res) {
    json body, ret;
    std::string user;
    try {
        body = json::parse(req.body);
        user = body.at("user").string_ref();
    } catch (std::exception& e) {
        LOG << e.what();
        return;
    }

    std::string sql = "select * from book";

    std::vector<std::vector<std::string>> result;
    ret["data"] = nullptr;
    if (user == "liph") {
        result = db[0].query(sql.c_str());
        ::to_json(result, ret["data"]);
    } else if (user == "jasmine") {
        result = db[1].query(sql.c_str());
        ::to_json(result, ret["data"]);
    }

    res.set_content(ret.to_string(), "application/json");
    res.set_header("Access-Control-Allow-Origin", "*");
}

void MyLibrary(const httplib::Request&, httplib::Response& res) {
    json ret;
    std::string sql = "select * from library";

    std::vector<std::vector<std::string>> result;
    ret["data"] = nullptr;
    result = db[2].query(sql.c_str());
    ::to_json(result, ret["data"]);
    res.set_content(ret.to_string(), "application/json");
    res.set_header("Access-Control-Allow-Origin", "*");
}

void AddMovie(const httplib::Request& req, httplib::Response& res) {
    json body, ret;
    std::string title, year, rating, douban, imdb, user;
    try {
        body = json::parse(req.body);
        title = body.at("title").string_ref();
        year = body.at("year").string_ref();
        rating = body.at("rating").string_ref();
        douban = body.at("douban").string_ref();
        imdb = body.at("imdb").string_ref();
        user = body.at("user").string_ref();
    } catch (...) {
        LOG << "json::parse error";
        return;
    }

    std::stringstream ss;
    ss << "INSERT INTO movie(title, year, rating, douban_item, imdb_item, "
          "record_date) ";
    ss << "values(\"" << title << "\", " << year << ", " << rating << ", \"" << douban << "\", \"" << imdb
       << "\", date('now','localtime'));";

    int db_ret = SQLITE_OK;
    if (user == "liph") {
        db_ret = db[0].execute(ss.str().c_str());
    } else if (user == "jasmine") {
        db_ret = db[1].execute(ss.str().c_str());
    }
    LOG << ss.str();

    ret["status"] = db_ret == SQLITE_OK;
    res.set_content(ret.to_string(), "application/json");
    res.set_header("Access-Control-Allow-Origin", "*");
}

void AddBook(const httplib::Request& req, httplib::Response& res) {
    json body, ret;
    std::string title, rating, isbn, user;
    try {
        LOG << req.body;
        body = json::parse(req.body);
        title = body.at("title").string_ref();
        rating = body.at("rating").string_ref();
        isbn = body.at("isbn").string_ref();
        user = body.at("user").string_ref();
    } catch (...) {
        LOG << "json::parse error";
        return;
    }

    std::stringstream ss;
    ss << "INSERT INTO book(title, rating, isbn, record_date) ";
    /* ss << "values('" << title << "', " << rating << ", '" << isbn << "', datetime('now','localtime'));"; */
    ss << "values('" << title << "', " << rating << ", '" << isbn << "', date('now','localtime'));";

    int db_ret = SQLITE_OK;
    if (user == "liph") {
        db_ret = db[0].execute(ss.str().c_str());
    } else if (user == "jasmine") {
        db_ret = db[1].execute(ss.str().c_str());
    }
    LOG << ss.str();

    ret["status"] = db_ret == SQLITE_OK;
    res.set_content(ret.to_string(), "application/json");
    res.set_header("Access-Control-Allow-Origin", "*");
}

void AddLibrary(const httplib::Request& req, httplib::Response& res) {
    json body, ret;
    std::string title, origin_title, author, publisher, translator, isbn, douban, tag;
    int publish_year;
    try {
        LOG << req.body;
        body = json::parse(req.body);
        title = body.at("title").string_ref();
        origin_title = body.at("origin_title").string_ref();
        author = body.at("author").string_ref();
        publisher = body.at("publisher").string_ref();
        publish_year = body.at("publish_year").i64_ref();
        translator = body.at("translator").string_ref();
        isbn = body.at("isbn").string_ref();
        douban = body.at("douban").string_ref();
        tag = body.at("tag").string_ref();
    } catch (...) {
        LOG << "json::parse error";
        return;
    }

    std::stringstream ss;
    ss << "INSERT INTO library(title, origin_title, author, publisher, "
          "publish_year, translator, isbn, douban_item, tag) ";
    ss << "values('" << title << "', '" << origin_title << "', '" << author << "', '" << publisher << "', "
       << publish_year << ", '" << translator << "', '" << isbn << "', '" << douban << "', '" << tag << "');";

    int db_ret = db[2].execute(ss.str().c_str());
    LOG << ss.str();

    ret["status"] = db_ret == SQLITE_OK;
    res.set_content(ret.to_string(), "application/json");
    res.set_header("Access-Control-Allow-Origin", "*");
}

int main(int argc, char *argv[]) {
    liph::flags flags;
    flags.register_string_flag("host", "0.0.0.0");
    flags.register_int32_flag("port", 8000);
    flags.register_string_flag("db_path", "");
    if (!flags.parse_flags(argc, &argv)) {
        std::cerr << flags.help() << std::endl;
        return 1;
    }
    FLAGS_host = flags.string_ref("host");
    FLAGS_port = flags.int32_ref("port");
    FLAGS_db_path = flags.string_ref("db_path");
    if (FLAGS_db_path.empty()) {
        std::cerr << "db_path empty" << std::endl;
        std::cerr << flags.help() << std::endl;
        return 1;
    }

    init_db();

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
