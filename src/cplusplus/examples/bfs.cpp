#include <algorithm>
#include <cmath>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

/*
 * for test
 * linux: g++ -g bfs.cpp -lgtest -lpthread
 * win: g++ -g bfs.cpp -I${GTEST_PATH}/include -L${GTEST_PATH}/lib -lgtest
 * case filter: ./a.out | ./a.exe --gtest_filter=patterns
 */
#include "gtest/gtest.h"
using namespace std;

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

/*
 * 从上到下按层打印二叉树，同一层结点从左至右输出
 */
vector<vector<int>> PrintTree(TreeNode *tree) {
    vector<vector<int>> ans;
    deque<TreeNode *> q;
    if (tree) q.push_back(tree);
    while (!q.empty()) {
        int n = q.size();
        vector<int> vc(n);
        for (int i = 0; i < n; i++) {
            vc[i] = q.front()->val;
            if (q.front()->left) q.push_back(q.front()->left);
            if (q.front()->right) q.push_back(q.front()->right);
            q.pop_front();
        }
        ans.emplace_back(vc);
    }
    return ans;
}

TEST(PrintTree, case1) {
    TreeNode tree[] = {8, 6, 10, 5, 7, 9, 11};
    tree[0].left = &tree[1];
    tree[0].right = &tree[2];
    tree[1].left = &tree[3];
    tree[1].right = &tree[4];
    tree[2].left = &tree[5];
    tree[2].right = &tree[6];

    vector<vector<int>> ans = {{8}, {6, 10}, {5, 7, 9, 11}};
    EXPECT_EQ(ans, PrintTree(&tree[0]));
}

/*
 * 给你一个由 '1'（陆地）和 '0'（水）组成的的二维网格，请你计算网格中岛屿的数量。
 * 岛屿总是被水包围，并且每座岛屿只能由水平方向和/或竖直方向上相邻的陆地连接形成。
 * 此外，你可以假设该网格的四条边均被水包围。
 * grid.size > 0, grid[i].size > 0
 * grid[i][j] 的值为 '0' 或 '1'
 */

void visitGrid(vector<vector<char>>& grid, deque<pair<int, int>>& q, int x, int y) {
    int m = grid.size(), n = grid[0].size();
    if (x >= 0 && x < m && y >= 0 && y < n && grid[x][y] == '1') {
        grid[x][y] = '0';
        q.push_back(make_pair(x, y));
    }
}

int numIslands(vector<vector<char>>& grid) {
    int ans = 0;
    int m = grid.size(), n = grid[0].size();
    deque<pair<int, int>> q;
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] != '1') continue;
            visitGrid(grid, q, i, j);
            while (!q.empty()) {
                for (size_t k = 0; k < q.size(); k++) {
                    int x = q.front().first;
                    int y = q.front().second;
                    q.pop_front();
                    visitGrid(grid, q, x + 1, y);
                    visitGrid(grid, q, x - 1, y);
                    visitGrid(grid, q, x, y + 1);
                    visitGrid(grid, q, x, y - 1);
                }
            }
            ans++;
        }
    }
    return ans;
}

TEST(numIslands, case1) {
    vector<vector<char>> grid{
            {'1', '1', '0', '0', '0'}, {'1', '1', '0', '0', '0'}, {'0', '0', '1', '0', '0'}, {'0', '0', '0', '1', '1'}};
    EXPECT_EQ(numIslands(grid), 3);
}

/*
 * 最小跳跃次数
 * 位置 i 可以跳到位置 i + jump[i] 或 [0, i) 的任意位置
 * 当跳的位置 i >= jump.size() 时成功
 * 初始位置为0，求成功的最小跳跃次数
 * jump.size() > 0
 */
inline void visit(vector<bool>& visited, queue<int>& q, int i) {
    if (!visited[i]) {
        visited[i] = true;
        q.push(i);
    }
}

int minJump(vector<int>& jump) {
    vector<bool> visited(jump.size());
    int ans = 1;
    queue<int> q;
    q.push(jump[0]);
    visited[jump[0]] = true;
    int prev = 0;
    for (;;) {
        int size = q.size();
        for (int j = 0; j < size; j++) {
            int cur = q.front();
            q.pop();
            if (cur + jump[cur] >= jump.size()) return ans + 1;
            visit(visited, q, cur + jump[cur]);
            for (int i = cur - 1; i > prev; i--) visit(visited, q, i);
            prev = max(prev, cur);
        }
        ans++;
    }
    return ans;
}

TEST(minJump, case1) {
    vector<int> jump{2, 5, 1, 1, 1, 1};
    EXPECT_EQ(minJump(jump), 3);

    vector<int> jump2{3, 7, 6, 1, 4, 3, 7, 8, 1, 2, 8, 5, 9, 8, 3, 2, 7, 5, 1, 1};
    EXPECT_EQ(minJump(jump2), 6);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
