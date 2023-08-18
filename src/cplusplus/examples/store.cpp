#include <filesystem>

#include "liph/liph.h"

namespace liph::store {

class segment {
public:
    segment(int id = 0) : id_(id) {}
    int id() const { return id_; }

    void init(const std::string& pathname) {
        file_.open(pathname, std::ios_base::binary | std::ios_base::in);
        std::string key;
        uint32_t value_offset;
        int i = 0;
        for (; read_one(key, value_offset); i++) {
            if (i % sparse_count == 0) {
                map_.insert(std::make_pair(std::move(key), value_offset));
            }
        }
        if (i % sparse_count != 0) {
            map_.insert(std::make_pair(std::move(key), value_offset));
        }
        file_.clear();
    }

    bool read_one(std::string& key, uint32_t& value_offset) {
        uint32_t length = 0;
        file_.read(reinterpret_cast<char *>(&length), sizeof(length));
        if (!file_.good()) {
            return false;
        }
        key.resize(length);
        file_.read(const_cast<char *>(key.data()), length);
        value_offset = file_.tellg();
        file_.read(reinterpret_cast<char *>(&length), sizeof(length));
        file_.seekg(length, std::ios_base::cur);
        return file_.good();
    }

    void dump(const std::string& pathname, const std::map<std::string, std::string>& data,
            const std::set<std::string>& del) {
        std::ofstream out{pathname, std::ios_base::binary | std::ios_base::trunc};
        for (const auto& x : data) {
            uint32_t length = x.first.size();
            out.write(reinterpret_cast<char *>(&length), sizeof(length));
            if (length != 0) {
                out.write(x.first.data(), length);
            }
            length = del.contains(x.first) ? -1 : x.second.size();
            out.write(reinterpret_cast<char *>(&length), sizeof(length));
            if (length != 0 && length != -1) {
                out.write(x.second.data(), length);
            }
        }
    }

    bool get_value(std::string& value, uint32_t offset) {
        file_.seekg(offset);
        uint32_t length = 0;
        file_.read(reinterpret_cast<char *>(&length), sizeof(length));
        if (!file_.good()) {
            return false;
        }
        if (length == -1) {  // deleted
            return false;
        }
        value.resize(length);
        file_.read(const_cast<char *>(value.data()), length);
        return file_.good();
    }

    bool skip_value(uint32_t offset) {
        file_.seekg(offset);
        uint32_t length = 0;
        file_.read(reinterpret_cast<char *>(&length), sizeof(length));
        if (!file_.good()) {
            return false;
        }
        file_.seekg(length, std::ios_base::cur);
        return file_.good();
    }

    bool get(const std::string& key, std::string& value) {
        auto it = map_.lower_bound(key);
        if (it == map_.end()) {
            return false;
        }
        if (it->first == key) {
            return get_value(value, it->second);
        }
        if (it == map_.begin()) return false;
        auto it2 = it;
        --it2;
        skip_value(it2->second);
        std::string key_tmp;
        uint32_t offset_tmp;
        while (read_one(key_tmp, offset_tmp)) {
            if (key_tmp == key) {
                return get_value(value, offset_tmp);
            }
            if (key_tmp == it->first) {
                break;
            }
        }
        return false;
    }

    bool operator<(const segment& other) { return id() < other.id(); }

private:
    int id_;
    static const int sparse_count = 16;
    std::ifstream file_;
    std::map<std::string, uint32_t> map_;
};

class db {
public:
    db(const std::string& store_path) : store_path_(store_path) {}
    ~db() {
        if (!mem_map_.empty()) {
            dump();
        }
    }

    bool init() {
        for (auto& p : std::filesystem::directory_iterator(store_path_)) {
            auto path = p.path().string();
            auto name = basename(path);
            if (startswith(name, "segment_")) {
                std::string id_str = name.substr(std::string("segment_").size());
                int id = -1;
                try {
                    id = std::stoi(id_str);
                } catch (...) {
                    continue;
                }
                if (id >= 0) {
                    segment seg(id);
                    seg.init(path);
                    segments_[id] = std::move(seg);
                }
            }
        }
        return true;
    }

    bool get(const std::string& key, std::string& value) {
        auto del_it = del_map_.find(key);
        if (del_it != del_map_.end()) {
            return false;
        }

        auto mem_it = mem_map_.find(key);
        if (mem_it != mem_map_.end()) {
            value = mem_it->second;
            return true;
        }
        for (auto it = segments_.rbegin(); it != segments_.rend(); ++it) {
            if (it->second.get(key, value)) {
                return true;
            }
        }

        return false;
    }

    bool put(const std::string& key, const std::string& value) {
        auto del_it = del_map_.find(key);
        if (del_it != del_map_.end()) {
            del_map_.erase(del_it);
        }

        mem_map_[key] = value;
        if (mem_map_.size() >= mem_map_max_size_) {
            dump();
        }
        return true;
    }

    void del(const std::string& key) { del_map_.insert(key); }

private:
    void dump() {
        int id = 0;
        if (!segments_.empty()) {
            id = segments_.rbegin()->first + 1;
        }
        segment seg(id);
        seg.dump(store_path_ + "/segment_" + std::to_string(id), mem_map_, del_map_);
        mem_map_.clear();
        seg.init(store_path_ + "/segment_" + std::to_string(id));
        segments_[id] = std::move(seg);
    }

private:
    std::map<int, segment> segments_;

    std::string store_path_;
    std::map<std::string, std::string> mem_map_;
    std::set<std::string> del_map_;
    static const int mem_map_max_size_ = 128;
};

}  // namespace liph::store

int main() {
    liph::store::db db("/tmp/store");
    D(db.init());
    for (int i = 100; i < 1000; i++) {
        if (!db.put("key_" + std::to_string(i), "value_" + std::to_string(i))) {
            liph::print("put err");
        }
    }
    std::string str;
    D(db.get("key_666", str));
    D(str);

    db.del("key_555");
    D(db.get("key_555", str));
}
