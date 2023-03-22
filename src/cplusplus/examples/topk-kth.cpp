#include <cassert>
#include <functional>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

/* data.size() * log(k) */
vector<int> topk(const vector<int>& data, size_t k) {
    if (k == 0) return vector<int>();
    if (data.size() <= k) return vector<int>(data.begin(), data.end());

    vector<int> ans;
    priority_queue<int, vector<int>, greater<int>> q(data.begin(), data.begin() + k);
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

int find_kth(const vector<int>& data, int k) {
    assert(data.size() > 0 && k >= 1 && k <= data.size());
    priority_queue<int> q(data.begin(), data.end());
    for (int i = 0; i < k - 1; i++) {
        q.pop();
    }
    return q.top();
}

int main() {
    vector<int> data{1, 3, 2, 3};
    vector<int> ans = topk(data, 2);
    cout << ans.size() << ": " << ans[0] << ", " << ans[1] << endl;
    //=>2: 3, 3
    cout << find_kth(data, 1) << endl;  //=>3
    cout << find_kth(data, 4) << endl;  //=>1
}
