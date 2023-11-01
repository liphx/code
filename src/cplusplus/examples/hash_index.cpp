#include <cstdint>
#include <filesystem>
#include <fstream>
#include <functional>
#include <string>
#include <utility>
#include <vector>

namespace liph {

class allocator {
public:
    using offset_type = std::pair<uint32_t, uint32_t>;
    using size_type = uint32_t;
    static constexpr offset_type null = {-1, -1};

public:
    allocator(size_type storage_capacity, size_type string_max_size)
            : string_max_size_(string_max_size), string_used_size_(0) {
        storage_.reserve(storage_capacity);
    }

    ~allocator() = default;

    offset_type alloc(size_type size) {
        if (size > string_max_size_) return null;
        if (storage_.empty() || string_used_size_ + size > string_max_size_) {
            storage_.emplace_back();
            storage_.back().reserve(string_max_size_);
            string_used_size_ = 0;
        }
        storage_.back().append(size, '\0');
        offset_type ret{storage_.size() - 1, string_used_size_};
        string_used_size_ = storage_.back().size();
        return ret;
    }

    offset_type alloc(const void *ptr, size_type size) {
        if (size > string_max_size_) return null;
        if (storage_.empty() || string_used_size_ + size > string_max_size_) {
            storage_.emplace_back();
            storage_.back().reserve(string_max_size_);
            string_used_size_ = 0;
        }
        storage_.back().append(reinterpret_cast<const char *>(ptr), size);
        offset_type ret{storage_.size() - 1, string_used_size_};
        string_used_size_ = storage_.back().size();
        return ret;
    }

    void free(offset_type) = delete;  // do not free

    void *address(offset_type offset) const {
        if (offset == null || offset.first >= storage_.size() || offset.second >= storage_[offset.first].size())
            return nullptr;
        return const_cast<char *>(storage_[offset.first].data() + offset.second);
    }

    bool load(std::filesystem::path path) {
        std::ifstream in{path};
        if (!in.is_open()) return false;
        storage_.clear();
        if (!in.read(reinterpret_cast<char *>(&string_max_size_), sizeof(string_max_size_))) return false;
        uint32_t storage_capacity;
        if (!in.read(reinterpret_cast<char *>(&storage_capacity), sizeof(storage_capacity))) return false;
        storage_.resize(storage_capacity);
        uint32_t string_size;
        for (uint32_t i = 0; i < storage_capacity; i++) {
            if (!in.read(reinterpret_cast<char *>(&string_size), sizeof(string_size))) return false;
            if (string_size > string_max_size_) return false;
            storage_[i].resize(string_size);
            if (!in.read(storage_[i].data(), string_size)) return false;
        }
        string_used_size_ = string_size;
        return true;
    }

    bool dump(std::filesystem::path path) const {
        std::ofstream out{path};
        if (!out.is_open()) return false;
        out.write(reinterpret_cast<const char *>(&string_max_size_), sizeof(string_max_size_));
        uint32_t storage_capacity = storage_.size();
        out.write(reinterpret_cast<const char *>(&storage_capacity), sizeof(storage_capacity));
        for (uint32_t i = 0; i < storage_capacity; i++) {
            uint32_t string_size = storage_[i].size();
            out.write(reinterpret_cast<const char *>(&string_size), sizeof(string_size));
            out.write(storage_[i].data(), string_size);
        }
        return out.good();
    }

private:
    std::vector<std::string> storage_;
    size_type string_max_size_;
    size_type string_used_size_;
};

template <class T>
uint64_t hash(const T& t) {
    return std::hash<T>{}(t);
}

template <class T>
struct cast {
    std::pair<const void *, allocator::size_type> to(const T& t) {
        return {&t, static_cast<allocator::size_type>(sizeof(t))};
    }

    T *from(const void *ptr) { return reinterpret_cast<T *>(const_cast<void *>(ptr)); }
};

template <>
struct cast<std::string> {
    std::pair<const void *, allocator::size_type> to(const std::string& str) { return {str.data(), str.size() + 1}; }

    const char *from(const void *ptr) { return reinterpret_cast<const char *>(ptr); }
};

template <class K, class V>
class hash_map {
public:
    struct entry {
        uint64_t key;
        allocator::offset_type offset;
        entry(uint64_t key_, allocator::offset_type offset_) : key(key_), offset(offset_) {}
    };

public:
    hash_map() : allocator_(64, 1024 * 1024) {}
    ~hash_map() = default;

    allocator::offset_type insert(const K& k, const V& v) {
        auto [ptr, size] = cast<V>().to(v);
        auto offset = allocator_.alloc(ptr, size);
        uint64_t key = hash(k);
        entries_.emplace_back(key, offset);
        return offset;
    }

    void build() {
        // entries_.size() == entry_idx_.size()
        // entries_[entry_idx_[i]].key % size >= i
        uint32_t entry_size = entries_.size();
        if (entry_size == 0) return;
        std::sort(entries_.begin(), entries_.end(), [entry_size](const entry& x, const entry& y) {
            uint32_t a = x.key % entry_size;
            uint32_t b = y.key % entry_size;
            return a == b ? x.key < y.key : a < b;
        });
        entry_idx_.resize(entry_size);
        entry_idx_[0] = 0;
        size_t j = 0;
        for (size_t i = 1; i < entry_size; ++i) {
            while (j < entry_size && entries_[j].key % entry_size == i - 1) {
                ++j;
            }
            entry_idx_[i] = j;
        }
    }

    const entry *find(const K& k) const {
        uint64_t key = hash(k);
        uint32_t entry_size = entries_.size();
        if (entry_size == 0) return nullptr;
        uint32_t i = key % entry_size;
        uint32_t begin = entry_idx_[i];
        uint32_t end = i + 1 >= entry_size ? entry_size : entry_idx_[i + 1];
        auto iter = std::lower_bound(entries_.data() + begin, entries_.data() + end, key,
                [](const entry& e, uint64_t key) { return e.key < key; });
        if (iter == entries_.data() + end || iter->key != key) return nullptr;
        return iter;
    }

    auto get(const entry *e) const -> decltype(reinterpret_cast<cast<V> *>(0)->from("")) {
        void *addr = e ? allocator_.address(e->offset) : nullptr;
        return cast<V>().from(addr);
    }

    allocator& get_allocator() { return allocator_; }

private:
    allocator allocator_;
    std::vector<entry> entries_;
    std::vector<uint32_t> entry_idx_;
};

}  // namespace liph

using namespace liph;
#include <iostream>

int main() {
    {
        hash_map<int, int> map;
        map.insert(0, 100);
        map.insert(1, 101);
        map.insert(10, 102);
        map.insert(100, 103);
        map.build();
        auto *ptr = map.find(100);
        std::cout << *map.get(ptr) << std::endl;
    }
    {
        hash_map<int, std::string> map;
        map.insert(0, "hello");
        map.insert(1, "42");
        map.insert(10, "abc");
        map.insert(100, "test");
        map.build();
        auto *ptr = map.find(100);
        std::cout << map.get(ptr) << std::endl;
    }
    {
        struct A {
            int32_t i;
            allocator::offset_type offset;
        };
        hash_map<uint64_t, std::string> text;
        hash_map<uint64_t, A> map;
        std::vector<std::string> data{"abc", "123", "hello", "test"};
        for (int i = 0; i < 4; i++) {
            uint64_t key = hash(data[i]);
            auto offset = text.insert(key, data[i]);
            A a{i, offset};
            map.insert(i, a);
        }
        map.build();
        auto *ptr = map.find(2);
        A *a = map.get(ptr);
        std::cout << a->i << std::endl;
        std::cout << (const char *)text.get_allocator().address(a->offset) << std::endl;
    }
}

int main_(int argc, char **argv) {
    if (argc == 1) {
        allocator alloc(16, 1024);
        int n = 42;
        auto offset = alloc.alloc(&n, sizeof(n));
        void *ptr = alloc.address(offset);
        std::cout << *(int *)ptr << std::endl;
        std::cout << offset.first << " " << offset.second << std::endl;
    } else if (argc == 2 && std::string(argv[1]) == "dump") {
        allocator alloc(16, 1024);
        int n = 42;
        alloc.alloc(&n, sizeof(n));
        std::cout << alloc.dump("test.data") << std::endl;
    } else if (argc == 2 && std::string(argv[1]) == "load") {
        allocator alloc(0, 0);
        std::cout << alloc.load("test.data") << std::endl;
        allocator::offset_type offset{0, 0};
        void *ptr = alloc.address(offset);
        std::cout << *(int *)ptr << std::endl;
    }
    return 0;
}
