#ifndef LIPH_ALGORITHM_H_
#define LIPH_ALGORITHM_H_

#include <cassert>
#include <functional>
#include <queue>
#include <set>
#include <vector>

namespace liph {

/// Erases the specified elements from v, according to index in idx
template <class T>
void erase(std::vector<T>& v, const std::set<size_t>& idx) {
    if (idx.empty()) return;
    auto it = idx.begin();
    size_t size = 0;
    for (size_t i = 0; i < v.size(); ++i) {
        if (it != idx.end() && i == *it) {
            ++it;
        } else {
            if (size != i) {
                v[size] = std::move(v[i]);
            }
            ++size;
        }
    }
    v.resize(size);
}

template <class T>
std::vector<T> topk(const std::vector<T>& data, size_t k) {
    if (k == 0) return {};
    if (data.size() <= k) return data;

    std::vector<T> ans;
    std::priority_queue<T, std::vector<T>, std::greater<T>> q(data.begin(), data.begin() + k);
    for (size_t i = k; i < data.size(); i++) {
        if (data[i] > q.top()) {
            q.pop();
            q.push(data[i]);
        }
    }
    for (size_t i = 0; i < k; i++) {
        ans.emplace_back(q.top());
        q.pop();
    }
    return ans;
}

template <class T>
T find_kth(const std::vector<T>& data, size_t k) {
    assert(data.size() > 0 && k >= 1 && k <= data.size());
    std::priority_queue<T> q(data.begin(), data.end());
    for (size_t i = 0; i < k - 1; i++) {
        q.pop();
    }
    return q.top();
}

template <class T>
int binary_search(T a[], int n, T val) {
    int i = 0, j = n, mid;
    while (i < j) {
        mid = i + (j - i) / 2;
        if (a[mid] == val) return mid;
        if (a[mid] < val) {
            i = mid + 1;
        } else {
            j = mid;
        }
    }
    return -1;
}

}  // namespace liph

#endif  // LIPH_ALGORITHM_H_
