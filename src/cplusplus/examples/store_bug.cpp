#include <filesystem>

#include "liph/liph.h"

namespace liph {

class db {
public:
    db(std::string pathname) : path(std::move(pathname)) {}
    ~db() { stream.sync(); }

    bool init();
    bool get(uint64_t key, std::string& value);
    bool put(uint64_t key, const std::string& value);
    bool del(uint64_t key);
    std::unordered_map<uint64_t, std::string> dump();

private:
    std::string path;
    std::fstream stream;

    std::unordered_map<uint64_t, uint64_t> map;      // key -> offset(uint32_t) + length(uint32_t)
    std::unordered_map<uint64_t, uint32_t> map_idx;  // key -> index offset
    uint32_t index_header = 0;                       // start offset of current index
    int current_index = index_size;                  // index of next element where be put
    uint32_t end = 0;                                // end offset
    static const int index_size = 128;               // element nums in one header
    static const int index_element_size = 16;        // key(uint64_t) + offset(uint32_t) + length(uint32_t)
};

bool db::init() {
    if (path.empty()) {
        return false;
    }
    if (!std::filesystem::exists(path)) {
        std::ofstream o(path);
    }
    stream.open(path, std::ios_base::in | std::ios_base::out | std::ios_base::binary);
    if (!stream.is_open()) {
        return false;
    }
    stream.seekg(0, std::ios_base::end);
    end = stream.tellg();

    uint32_t current = 0;
    while (end >= current + index_size * index_element_size) {
        liph::print(current, end, index_size * index_element_size);
        char buffer[index_size * index_element_size];
        stream.seekg(current);
        stream.read(buffer, sizeof(buffer));
        uint64_t key, u64;
        for (int i = 0; i < index_size; i++) {
            uint64_t *pkey = reinterpret_cast<uint64_t *>(buffer + i * index_element_size);
            key = *pkey;
            u64 = *(pkey + 1);
            if (u64 != 0) {
                map[key] = u64;
                map_idx[key] = current + i * index_element_size;
                current = std::max(current, static_cast<uint32_t>(u64 >> 32) + static_cast<uint32_t>(u64));
            }
        }
    }
    return true;
}

bool db::get(uint64_t key, std::string& value) {
    auto it = map.find(key);
    if (it == map.end()) return false;
    uint32_t offset = static_cast<uint32_t>(it->second >> 32);
    uint32_t length = static_cast<uint32_t>(it->second);
    stream.seekg(offset);
    auto n = value.size();
    value.resize(n + length);
    stream.read(const_cast<char *>(value.data() + n), length);
    return stream.good();
}

bool db::put(uint64_t key, const std::string& value) {
    auto it = map.find(key);
    if (it == map.end()) {  // add
        if (current_index == index_size) {
            char buffer[index_size * index_element_size]{0};
            stream.seekp(end);
            stream.write(buffer, sizeof(buffer));
            index_header = end;
            end = stream.tellp();
            current_index = 0;
        }
        stream.seekp(index_header + index_element_size * current_index);
        map_idx[key] = stream.tellp();
        ++current_index;
        stream.write(reinterpret_cast<char *>(&key), sizeof(key));
        uint64_t u64 = (static_cast<uint64_t>(end) << 32) + static_cast<uint64_t>(value.length());
        map[key] = u64;
        stream.write(reinterpret_cast<char *>(&u64), sizeof(u64));
        stream.seekp(end);
        stream.write(value.data(), value.length());
        end = stream.tellp();
    } else {  // update
        uint64_t u64 = (static_cast<uint64_t>(end) << 32) + static_cast<uint64_t>(value.length());
        it->second = u64;
        uint32_t pos = map_idx[key];
        stream.seekp(pos + sizeof(key));
        stream.write(reinterpret_cast<char *>(&u64), sizeof(u64));
        stream.seekp(end);
        stream.write(value.data(), value.length());
        end = stream.tellp();
    }
    stream.flush();
    return stream.good();
}

bool db::del(uint64_t key) {
    auto it = map.find(key);
    if (it == map.end()) {
        return false;
    }
    uint32_t pos = map_idx[key];
    stream.seekp(pos + sizeof(key));
    uint64_t u64 = 0;
    stream.write(reinterpret_cast<char *>(&u64), sizeof(u64));
    map.erase(key);
    map_idx.erase(key);
    return stream.good();
}

std::unordered_map<uint64_t, std::string> db::dump() {
    std::unordered_map<uint64_t, std::string> ret;
    for (auto it = map.begin(); it != map.end(); ++it) {
        auto key = it->first;
        auto& value = ret[key];
        get(key, value);
    }
    return ret;
}

}  // namespace liph

int main() {
    liph::db db("tmp.db");
    D(db.init());
    liph::print(db.dump());
    std::vector<std::string> vc{"abc001", "bcd002", "cdef0003"};
    for (int i = 0; i < vc.size(); i++) D(db.put(i, vc[i]));
    liph::print(db.dump());
    D(db.put(0, "000"));
    liph::print(db.dump());
    D(db.del(0));
    liph::print(db.dump());
    D(db.del(0));
    std::filesystem::remove("tmp.db");
}
