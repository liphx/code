#include <atomic>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <mutex>
#include <thread>
#include <vector>

template <class T>
class Deque {
public:
    Deque() = default;
    ~Deque() = default;

    Deque(const Deque&) = delete;
    Deque& operator=(const Deque&) = delete;

    void Push(const T& t) {
        std::lock_guard<std::mutex> lock(mtx_);
        deq_.push_back(t);
    }

    bool Pop(T& t) {
        std::lock_guard<std::mutex> lock(mtx_);
        if (deq_.empty()) {
            return false;
        }
        t = deq_.front();
        deq_.pop_front();
        return true;
    }

    size_t Size() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return deq_.size();
    }

    bool Empty() const { return Size() == 0; }

private:
    std::deque<T> deq_;
    mutable std::mutex mtx_;
};

/* template <class K, class V> */
/* class Map { */
/* public: */
/*     Map() = default; */
/*     ~Map() = default; */

/*     Map(const Map&) = delete; */
/*     Map& operator=(const Map&) = delete; */

/*     void insert(const K& k, const V& v) { */
/*         std::lock_guard<std::mutex> lock(mtx_); */
/*         map_.insert(std::make_pair(k, v)); */
/*     } */

/*     void erase(const K& k) { */
/*         std::lock_guard<std::mutex> lock(mtx_); */
/*         map_.erase(k); */
/*     } */

/*     bool get(const K& k, V& v) const { */
/*         std::lock_guard<std::mutex> lock(mtx_); */
/*         auto it = map_.find(k); */
/*         if (it != map_.end()) { */
/*             v = it->second; */
/*             return true; */
/*         } */
/*         return false; */
/*     } */

/*     V get(const K& k) const { */
/*         V v; */
/*         get(k, v); */
/*         return v; */
/*     } */

/*     size_t size() const { */
/*         std::lock_guard<std::mutex> lock(mtx_); */
/*         return map_.size(); */
/*     } */

/* private: */
/*     std::map<K, V> map_; */
/*     mutable std::mutex mtx_; */
/* }; */

/* using FileGroup = Map<std::string, Deque<std::string>>; */
using FileGroup = std::map<std::string, Deque<std::string>>;
FileGroup files;
const std::vector<std::string> group_names{"a", "b"};
const size_t group_size = group_names.size();
std::vector<std::ifstream> ifs_list(group_size);
std::vector<std::string> ifs_name_list(group_size);

void DealFileGroup() {
    std::string str;
    std::vector<std::ifstream::pos_type> pos_list(group_size);
    while (true) {
        size_t eof_count = 0;
        for (size_t i = 0; i < group_size; ++i) {
            auto& ifs = ifs_list[i];
            auto& deq = files[group_names[i]];
            if (!ifs.is_open()) {  // pop deq and open file
                std::string file;
                if (!deq.Pop(file)) {
                    eof_count++;
                    continue;
                }
                std::cout << "open file " << file << std::endl;
                ifs.open(file);
                ifs_name_list[i] = file;
                pos_list[i] = ifs.tellg();
            }

            if (std::getline(ifs, str)) {
                pos_list[i] = ifs.tellg();
                std::cout << ifs_name_list[i] << ": " << str << std::endl;
            } else if (!deq.Empty()) {
                std::cout << "close file " << ifs_name_list[i] << std::endl;
                ifs.close();  // done
            } else {
                eof_count++;
                ifs.clear();
                ifs.seekg(pos_list[i]);
            }
        }
        if (eof_count == group_size) {
            std::cout << "eof_count == group_size, sleep 1" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
}

int main() {
    auto& qa = files["a"];
    qa.Push("a1");
    qa.Push("a2");
    auto& qb = files["b"];
    qb.Push("b1");

    std::thread t1(DealFileGroup);
    std::thread t2([]() {
        std::this_thread::sleep_for(std::chrono::seconds(10));
        auto& deq = files["b"];
        deq.Push("b2");
    });
    t1.join();
    t2.join();
}
