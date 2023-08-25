#include <stack>

#include "liph/liph.h"
using namespace std;

namespace liph {

struct record {
    record(int x, int y, std::set<int>& nums) : x(x), y(y), nums(std::move(nums)) {
        select = *this->nums.begin();
        nums.insert(select);
    }

    int x;
    int y;
    int select;
    std::set<int> nums;

    bool select_next() {
        while (select < 9 && nums.find(++select) != nums.end()) {
            return true;
        }
        return false;
    }
};

struct sudoku {
    int data[9][9];

    bool input(const std::string& str) {
        if (str.length() < 81) return false;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                char ch = str[i * 9 + j];
                if (ch < '0' || ch > '9') {
                    return false;
                }
                data[i][j] = ch - '0';
            }
        }
        return true;
    }

    void show() const {
        std::string sep;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                int num = data[i][j];
                std::cout << sep << (num == 0 ? " " : std::to_string(num));
                sep = "|";
            }
            sep = "\n";
        }
        std::cout << "\n";
    }

    bool solve() {
        std::set<int> alternate[9][9];
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (data[i][j] == 0) {
                    alternate[i][j] = std::set<int>{1, 2, 3, 4, 5, 6, 7, 8, 9};
                } else if (data[i][j] > 0 && data[i][j] <= 9) {
                    alternate[i][j] = std::set<int>{data[i][j]};
                } else {
                    return false;
                }
            }
        }
        std::stack<record> st;
        st.emplace(0, 0, alternate[0][0]);
        while (true) {
            auto& top = st.top();
            bool ret = check((std::set<int> **)alternate);
            if (ret) {  // valid now, push next to stack
                int find_x, find_y;
                for (int j = top.y + 1; j < 9; j++) {
                    if (alternate[top.x][j].size() > 1) {
                        find_x = top.x;
                        find_y = j;
                        goto find;
                    }
                }
                for (int i = top.x + 1; i < 9; i++) {
                    for (int j = 0; j < 9; j++) {
                        if (alternate[i][j].size() > 1) {
                            find_x = i;
                            find_y = j;
                            goto find;
                        }
                    }
                }
                for (int i = 0; i < 9; i++) {
                    for (int j = 0; j < 0; j++) {
                        if (data[i][j] == 0) {
                            data[i][j] = *alternate[i][j].begin();
                        }
                    }
                }
                return true;
            find:
                st.emplace(find_x, find_y, alternate[find_x][find_y]);
            } else {  // not valid, pop
                while (!st.empty()) {
                    auto top = st.top();
                    st.pop();
                    if (top.select_next()) {
                        st.push(top);
                        break;
                    } else {
                        alternate[top.x][top.y].swap(top.nums);
                    }
                }
                if (st.empty()) {
                    return false;
                }
            }
        }
        return false;
    }

private:
    bool check(std::set<int> **alternate) {
        for (int i = 0; i < 9; i++) {  // check row
            std::set<int> tmp;
            int zero_count = 0;
            for (int j = 0; j < 9; j++) {
                if (alternate[i][j].size() > 1) {
                    zero_count++;
                } else {  // size == 1
                    tmp.insert(*alternate[i][j].begin());
                }
            }
            if (tmp.size() + zero_count < 9) {
                return false;
            }
        }
        for (int i = 0; i < 9; i++) {  // check col
            std::set<int> tmp;
            int zero_count = 0;
            for (int j = 0; j < 9; j++) {
                if (alternate[j][i].size() > 1) {
                    zero_count++;
                } else {  // size == 1
                    tmp.insert(*alternate[j][i].begin());
                }
            }
            if (tmp.size() + zero_count < 9) {
                return false;
            }
        }
        static int grid_begin[9][2] = {{1, 1}, {1, 4}, {1, 7}, {4, 1}, {4, 4}, {4, 7}, {7, 1}, {7, 4}, {7, 7}};
        for (int grid_idx = 0; grid_idx < 9; grid_idx++) {  // check grid
            int x = grid_begin[grid_idx][0];
            int y = grid_begin[grid_idx][1];
            std::set<int> tmp;
            int zero_count = 0;
            for (int i = x; i < x + 3; i++) {
                for (int j = y; j < y + 3; j++) {
                    for (int j = 0; j < 9; j++) {
                        if (alternate[j][i].size() > 1) {
                            zero_count++;
                        } else {  // size == 1
                            tmp.insert(*alternate[j][i].begin());
                        }
                    }
                }
            }
            if (tmp.size() + zero_count < 9) {
                return false;
            }
        }
        return true;
    }
};

}  // namespace liph

int main() {
    std::string str;
    liph::read_file("/tmp/sudoku.txt", str);
    liph::sudoku sudoku;
    if (!sudoku.input(str)) {
        std::cerr << "Invalid input: " << str << std::endl;
        exit(1);
    }
    if (sudoku.solve()) {
        sudoku.show();
    } else {
        std::cerr << "No solution!" << std::endl;
    }
}

#if 0
class Sudoku {
public:
    Sudoku(int mode = 0) : data(), mode(mode) {
        srand(time(0));

        while (!GenerateData()) {  // random
            Clear();
        }

        GenerateProblem(mode);
    }

    Sudoku(const array<array<int, 9>, 9>& data) { this->data = data; }

    bool Solve() {
        int unknown = 81;
        for (auto& row : data) unknown -= count(row.begin(), row.end(), 0);
        cout << unknown << endl;
        return true;
    }

    void Show() const {
        string sep;
        for (const auto& row : data) {
            for (int num : row) {
                cout << sep << (num == 0 ? " " : to_string(num));
                sep = "|";
            }
            sep = "\n";
        }
        cout << "\n";
    }

private:
    array<array<int, 9>, 9> data;
    int mode;  // 0 for easy, 1 for middle, 2 for hard
    static vector<vector<int>> grid_begin;

    void Clear() {
        for (auto& row : data) {
            row.fill(0);
        }
    }

    void SetNum(int num, int row, int col) { data[row - 1][col - 1] = num; }

    int GetNum(int row, int col) const { return data[row - 1][col - 1]; }

    int RandomNum(const vector<int>& nums) const {
        if (nums.empty()) return -1;
        return nums[rand() % nums.size()];
    }

    bool CanFillIn(int num, int row, int col) const {
        if (GetNum(row, col) != 0) return false;

        for (int i = 1; i <= 9; i++) {
            if (i != col && GetNum(row, i) == num) return false;
        }
        for (int i = 1; i <= 9; i++) {
            if (i != row && GetNum(i, col) == num) return false;
        }

        int grid_row = 1 + (row - 1) / 3 * 3;
        int grid_col = 1 + (col - 1) / 3 * 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int x = grid_row + i;
                int y = grid_col + j;
                if (x == row && y == col) continue;
                if (GetNum(x, y) == num) return false;
            }
        }

        return true;
    }

    bool GenerateData() {
        for (int num = 1; num <= 9; num++) {
            for (int row = 1; row <= 9; row++) {
                vector<int> nums;
                for (int col = 1; col <= 9; col++) {
                    if (CanFillIn(num, row, col)) {
                        nums.emplace_back(col);
                    }
                }
                int col = RandomNum(nums);
                if (col == -1) return false;
                SetNum(num, row, col);
            }
        }
        return true;
    }

    // if can be solved by one approach
    bool IsUnique() {
        Sudoku try_solve(data);
        return try_solve.Solve();
    }

    void SetUnknownIfUnique(int row, int col) {
        int origin = GetNum(row, col);
        if (origin == 0) return;
        SetNum(0, row, col);
        if (!IsUnique()) SetNum(origin, row, col);
    }

    void GenerateProblem(int mode) {
        // Randomly set one for each row, col, grid as unknown
        // repeat
        for (int times = mode; times > 0; times--) {
            for (int row = 1; row <= 9; row++) SetUnknownIfUnique(row, rand() % 9 + 1);
            for (int col = 1; col <= 9; col++) SetUnknownIfUnique(rand() % 9 + 1, col);
            for (int grid = 1; grid <= 9; grid++) {
                int row = grid_begin[grid - 1][0] + rand() % 3;
                int col = grid_begin[grid - 1][1] + rand() % 3;
                SetUnknownIfUnique(row, col);
            }
        }
    }
};
}
#endif
