#include <unistd.h>

#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>

#include "liph/terminal.h"

int arr[4][4];

struct p {
    int x;
    int y;
    p(int x, int y) : x(x), y(y) {}
};

p npos = {-1, -1};

p new2() {
    std::vector<p> free;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (arr[i][j] == 0) {
                free.emplace_back(i, j);
            }
        }
    }
    if (free.empty()) {
        return npos;
    }
    return free[rand() % free.size()];
}

int score() {
    int *p = &arr[0][0];
    return std::accumulate(p, p + 16, 0, [](int total, int x) { return x > 2 ? total + x : total; });
}

void init() {
    srand(0);
    p p1 = new2();
    arr[p1.x][p1.y] = 2;
    p p2 = new2();
    arr[p2.x][p2.y] = 2;
}

std::string color_normal = "\033[0m";
std::map<int, std::string> num2color = {{0, color_normal}, {2, "\033[;41m"}, {4, "\033[;42m"}, {8, "\033[;43m"},
        {16, "\033[;44m"}, {32, "\033[;45m"}, {64, "\033[;46m"}, {128, "\033[;48m"}, {256, "\033[;49m"}};

void show() {
    std::cout << "\033[2J";
    std::cout << "score: " << score() << std::endl;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::stringstream ss;
            ss.width(4);
            ss.setf(std::ios::right);
            if (arr[i][j] == 0)
                ss << "     ";
            else
                ss << arr[i][j] << " ";
            std::string str = num2color[arr[i][j]] + ss.str() + color_normal;
            std::cout << str;
        }
        std::cout << std::setw(4) << std::endl;
    }
}

// return 1 if change
int merge(int& a, int& b, int& c, int& d) {
    int out[4]{a, b, c, d};
    int i = 0;
    for (int j = 0; j < 4; ++j) {
        if (out[j] != 0) {
            out[i++] = out[j];
        }
    }
    for (; i < 4; ++i) {
        out[i] = 0;
    }
    if (out[0] == out[1]) {
        out[0] *= 2;
        out[1] = out[2];
        out[2] = out[3];
        out[3] = 0;
        if (out[1] == out[2]) {
            out[1] *= 2;
            out[2] = 0;
        }
    } else {
        if (out[1] == out[2]) {
            out[1] *= 2;
            out[2] = out[3];
            out[3] = 0;
        } else {
            if (out[2] == out[3]) {
                out[2] *= 2;
                out[3] = 0;
            }
        }
    }
    int ret = (a == out[0] && b == out[1] && c == out[2] && d == out[3]) ? 0 : 1;
    a = out[0];
    b = out[1];
    c = out[2];
    d = out[3];
    return ret;
}

void move(int di) {
    bool change = false;
    switch (di) {
    case 1:  // left
        for (int i = 0; i < 4; ++i) {
            if (merge(arr[i][0], arr[i][1], arr[i][2], arr[i][3]) == 1) change = true;
        }
        break;
    case 2:  // right
        for (int i = 0; i < 4; ++i) {
            if (merge(arr[i][3], arr[i][2], arr[i][1], arr[i][0]) == 1) change = true;
        }
        break;
    case 3:  // up
        for (int i = 0; i < 4; ++i) {
            if (merge(arr[0][i], arr[1][i], arr[2][i], arr[3][i]) == 1) change = true;
        }
        break;
    case 4:  // down
        for (int i = 0; i < 4; ++i) {
            if (merge(arr[3][i], arr[2][i], arr[1][i], arr[0][i]) == 1) change = true;
        }
        break;
    default:
        assert(0);
    }
    if (change) {
        p p1 = new2();
        arr[p1.x][p1.y] = 2;
    }
    show();
}

int main() {
    init();
    show();
    while (true) {
        liph::echo_off(0);
        char ch = getchar();
        liph::echo_on(0);
        if (ch == EOF) break;
        switch (ch) {
        case 'a':
            move(1);
            break;
        case 'd':
            move(2);
            break;
        case 'w':
            move(3);
            break;
        case 's':
            move(4);
            break;
        default:
            break;
        }
    }
}
