#include <unistd.h>

#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

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

void show() {
    std::cout << "score: " << score() << std::endl;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            std::cout << std::setw(4) << arr[i][j];
        }
        std::cout << std::endl;
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
        if (out[2] == out[3]) {
            out[2] *= 2;
            out[3] = 0;
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
        show();
    }
}

int main() {
    init();
    show();
    char ch;
    setbuf(stdin, NULL);
    while ((ch = getchar()) != EOF) {
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
