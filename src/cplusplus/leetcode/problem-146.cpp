// https://leetcode.cn/problems/lru-cache/

#include "leetcode.h"

class LRUCache {
public:
    LRUCache(int capacity) : capacity(capacity) {}

    int get(int key) {
        if (it.find(key) == it.end()) return -1;
        auto kv = *it[key];
        data.erase(it[key]);
        data.emplace_front(kv);
        it[key] = data.begin();
        return kv.second;
    }

    void put(int key, int value) {
        if (it.find(key) != it.end()) {  // exist
            data.erase(it[key]);
            it.erase(key);
        } else if (data.size() == capacity) {  // full
            auto back = data.back();
            it.erase(back.first);
            data.pop_back();
        }
        data.emplace_front(make_pair(key, value));
        it.insert(make_pair(key, data.begin()));
    }

private:
    size_t capacity;
    list<pair<int, int>> data;
    unordered_map<int, list<pair<int, int>>::iterator> it;
};

int main() {
    LRUCache *obj = new LRUCache(2);
    obj->put(1, 1);
    obj->put(2, 2);
    cout << obj->get(1) << endl;  // 1
    obj->put(3, 3);
    cout << obj->get(2) << endl;  // -1
    obj->put(4, 4);
    cout << obj->get(1) << endl;  // -1
    cout << obj->get(3) << endl;  // 3
    cout << obj->get(4) << endl;  // 4
}
