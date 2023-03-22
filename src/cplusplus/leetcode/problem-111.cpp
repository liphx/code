// https://leetcode.cn/problems/minimum-depth-of-binary-tree/

#include "leetcode.h"

class Solution {
public:
    int minDepth(TreeNode *root) {
        if (root == nullptr) return 0;
        if (root->left == nullptr && root->right == nullptr) return 1;
        if (root->left != nullptr && root->right == nullptr) return 1 + minDepth(root->left);
        if (root->right != nullptr && root->left == nullptr) return 1 + minDepth(root->right);
        return 1 + min(minDepth(root->left), minDepth(root->right));
    }
};

int main() {
    TreeNode a(1), b(2);
    a.left = &b;
    cout << Solution().minDepth(&a) << endl;  // 2
}
