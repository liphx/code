// https://leetcode.cn/problems/binary-tree-maximum-path-sum/

#include "leetcode.h"

class Solution {
public:
    int maxPathSum(TreeNode *root) {
        if (!root->left && !root->right) return root->val;
        int l, r, val = maxPath(root);
        if (root->left) l = maxPathSum(root->left);
        if (root->right) r = maxPathSum(root->right);
        if (root->left && !root->right) return max({val, l});
        if (!root->left && root->right) return max({val, r});
        return max({val, l, r});
    }

private:
    int maxPath(TreeNode *root) {
        int val = root->val;
        if (root->left) {
            int l = maxPathOne(root->left);
            if (l > 0) val += l;
        }
        if (root->right) {
            int r = maxPathOne(root->right);
            if (r > 0) val += r;
        }
        return val;
    }

    int maxPathOne(TreeNode *root) {
        int l, r, val = root->val;
        if (root->left) l = maxPathOne(root->left);
        if (root->right) r = maxPathOne(root->right);
        if (root->left && !root->right && l > 0) return val + l;
        if (!root->left && root->right && r > 0) return val + r;
        if (root->left && root->right) return val + max({0, l, r});
        return val;
    }
};

int main() {}
