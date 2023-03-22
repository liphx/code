#include <algorithm>
#include <array>
#include <iostream>
#include <vector>
using namespace std;

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

vector<vector<int>> Sudoku::grid_begin = {{1, 1}, {1, 4}, {1, 7}, {4, 1}, {4, 4}, {4, 7}, {7, 1}, {7, 4}, {7, 7}};

int main(int argc, char **argv) {
    Sudoku sudoku;
    sudoku.Show();
    /* puts("Do you want..\n" */
    /*      "  1. Generate Problem\n" */
    /*      "  2. Solve Problem"); */
    /* int select; */
    /* cin >> select; */
    /* if (select == 1) { */
    /*     puts("Choose Difficulty Mode..\n" */
    /*         "  1. Easy\n" */
    /*         "  2. Middle\n" */
    /*         "  3. Hard"); */
    /*     int mode; */
    /*     cin >> mode; */
    /*     mode--; */
    /*     if (mode < 0 || mode > 2) { */
    /*         exit(0); */
    /*     } */
    /*     Sudoku sudoku(mode); */
    /*     sudoku.Show(); */
    /* } else if (select == 2) { */
    /*     puts("Input Your Problem..\n" */
    /*             "Your Input Is Like:\n" */
    /*             "5231896 7\n" */
    /*             "6 4523 81\n" */
    /*             "8  46352 \n" */
    /*             "4 29 7 68\n" */
    /*             "5 612 39 \n" */
    /*             "91 43 2  \n" */
    /*             "7  2  89 \n" */
    /*             "298 745  \n" */
    /*             "618957   \n"); */
    /*     array<array<int, 9>, 9> data; */
    /*     int num; */
    /*     int i = 0; */
    /*     while (i < 9) { */
    /*         for (int j = 0; j < 9; j++) { */
    /*             num = getchar(); */
    /*             if (num == '\n') { */
    /*                 break; */
    /*             } */
    /*             if (num == ' ') */
    /*                 num = 0; */
    /*             else if (num >= '1' || num <= '9') { */
    /*                 num -= '0'; */
    /*             } else { */
    /*                 puts("Invalid Number!"); */
    /*                 exit(0); */
    /*             } */

    /*             data[i][j] = num; */
    /*         } */
    /*         i++; */
    /*     } */
    /*     Sudoku sudoku(data); */
    /*     if (sudoku.Solve()) { */
    /*         puts("The Sudoku Can be Solved, The Answer is"); */
    /*         sudoku.Show(); */
    /*     } */
    /* } else { */
    /*     exit(0); */
    /* } */
}
