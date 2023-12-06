#include <algorithm>
#include <cmath>
#include <iostream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

/*
 * 输入一棵二叉树的根节点，求该树的深度。从根节点到叶节点依次经过的节点（含根、叶节点）形成树的一条路径，最长路径的长度为树的深度。
 * recursive
 */
int maxDepth(TreeNode *tree) {
    if (!tree) return 0;
    return 1 + max(maxDepth(tree->left), maxDepth(tree->right));
}

/*
 * use stack, dfs
 */
int maxDepth2(TreeNode *tree) {
    stack<TreeNode *> s;
    if (tree == nullptr) return 0;

    s.push(tree);
    int max_depth = 1;
    int depth = 1;
    TreeNode *node, *prev = tree;
    while (!s.empty()) {
        node = s.top();
        if (node->left && prev != node->left && prev != node->right) {
            s.push(node->left);
            depth++;
            max_depth = max(max_depth, depth);
        } else if (node->right && prev != node->right) {
            s.push(node->right);
            depth++;
            max_depth = max(max_depth, depth);
        } else {
            prev = s.top();
            s.pop();
            depth--;
        }
    }
    return max_depth;
}

int main(int argc, char **argv) {
    // [3,9,20,null,null,15,7]
    TreeNode tree[] = {3, 9, 20, -1, -1, 15, 7};
    tree[0].left = &tree[1];
    tree[0].right = &tree[2];
    tree[2].left = &tree[5];
    tree[2].right = &tree[6];

    /* EXPECT_EQ(maxDepth(&tree[0]), 3); */
    /* EXPECT_EQ(maxDepth2(&tree[0]), 3); */
}
