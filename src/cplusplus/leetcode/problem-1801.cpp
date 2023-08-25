// https://leetcode.cn/problems/number-of-orders-in-the-backlog/

#include "leetcode.h"

class Solution {
public:
    int getNumberOfBacklogOrders(vector<vector<int>>& orders) {
        multimap<int, int, greater<int>> buy;
        multimap<int, int, less<int>> sell;
        for (size_t i = 0; i < orders.size(); i++) {
            if (orders[i][2] == 0)
                f(orders, i, sell, buy);
            else
                f(orders, i, buy, sell);
        }
        long long ret = 0;
        for (size_t i = 0; i < orders.size(); i++) {
            ret += orders[i][1];
        }
        return ret % 1000000007;
    }

    template <class T1, class T2>
    void f(vector<vector<int>>& orders, size_t i, T1& sell, T2& buy) {
        int& price = orders[i][0];
        int& amount = orders[i][1];
        for (auto it = sell.begin(); it != sell.upper_bound(price);) {
            int& delay_amount = orders[it->second][1];
            if (delay_amount > amount) {
                delay_amount -= amount;
                amount = 0;
                break;
            }
            amount -= delay_amount;
            delay_amount = 0;
            it = sell.erase(it);
            if (amount == 0) break;
        }
        if (amount > 0) {
            buy.insert(make_pair(price, i));
        }
    }
};

int main() {
    vector<vector<int>> orders = {{10, 5, 0}, {15, 2, 1}, {25, 1, 1}, {30, 4, 0}};
    cout << Solution().getNumberOfBacklogOrders(orders) << endl;  // 6

    orders = {{30, 27, 1}, {18, 9, 1}, {11, 4, 0}, {21, 11, 0}, {1, 1, 1}, {24, 20, 1}, {15, 13, 1}, {13, 3, 0},
            {30, 11, 1}};
    cout << Solution().getNumberOfBacklogOrders(orders) << endl;  // 77

    orders = {{27, 30, 0}, {10, 10, 1}, {28, 17, 1}, {19, 28, 0}, {16, 8, 1}, {14, 22, 0}, {12, 18, 1}, {3, 15, 0},
            {25, 6, 1}};
    cout << Solution().getNumberOfBacklogOrders(orders) << endl;  // 82

    orders = {{7, 1000000000, 1}, {15, 3, 0}, {5, 999999995, 0}, {5, 1, 1}};
    cout << Solution().getNumberOfBacklogOrders(orders) << endl;  // 999999984
}
