// https://leetcode.cn/problems/expression-add-operators/

#include "leetcode.h"

class Solution {
public:
    vector<string> addOperators(string num, int target) {
        vector<string> ss, ans;
        addOperators(ss, num, "");
        for (const string& s : ss) {
            if (check(s, target)) ans.emplace_back(s);
        }
        return ans;
    }

private:
    void addOperators(vector<string>& ans, string_view num, const string& str) {
        if (num.length() == 1)
            ans.emplace_back(str + num[0]);
        else {
            string ss = str + num[0] + '+';
            string_view new_num = num.substr(1);
            addOperators(ans, new_num, ss);
            ss = str + num[0] + '-';
            addOperators(ans, new_num, ss);
            ss = str + num[0] + '*';
            addOperators(ans, new_num, ss);
            ss = str + num[0];
            addOperators(ans, new_num, ss);
        }
    }

    bool check(const string& str, int target) {
        vector<long long> nums;
        vector<char> ops;
        if (!split(nums, ops, str)) return false;
        long long a = nums[0], b = nums[1], c = nums[2];
        char op1 = ops[0], op2 = ops[1];
        int i = 3;
        while (true) {
            if (op1 == '*') {
                a *= b;
                b = c;
                op1 = op2;
            } else if (op2 == '*') {
                b *= c;
            } else {
                if (op1 == '+')
                    a += b;
                else
                    a -= b;
                b = c;
                op1 = op2;
            }
            if ((size_t)i == nums.size()) {
                if (op1 == '+') return a + b == target;
                if (op1 == '-') return a - b == target;
                return a * b == target;
            }
            op2 = ops[i - 1];
            c = nums[i++];
        }
    }

    bool split(vector<long long>& nums, vector<char>& ops, const string& str) {
        long long num = 0;
        bool first = true;
        for (char c : str) {
            if (c < '0' || c > '9') {
                ops.emplace_back(c);
                nums.emplace_back(num);
                num = 0;
                first = true;
            } else {
                if (!first) {
                    if (num == 0) return false;
                    num *= 10;
                }
                num += c - '0';
                first = false;
            }
        }
        if (!first) nums.emplace_back(num);
        if (nums.empty()) return false;
        nums.emplace_back(0);
        nums.emplace_back(0);
        ops.emplace_back('+');
        ops.emplace_back('+');
        return true;
    }
};

int main() {
    cout << Solution().addOperators("123", 6) << endl;            // [1+2+3, 1*2*3]
    cout << Solution().addOperators("232", 8) << endl;            // [2+3*2, 2*3+2]
    cout << Solution().addOperators("3456237490", 9191) << endl;  // []
    cout << Solution().addOperators("105", 5) << endl;            // [1*0+5, 10-5]
    cout << Solution().addOperators("00", 0) << endl;             // [0+0, 0-0, 0*0]
    cout << Solution().addOperators("1000000009", 9) << endl;
    cout << Solution().addOperators("2147483647", 2147483647) << endl;  // [2147483647]
}
