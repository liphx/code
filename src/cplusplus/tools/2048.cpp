#include "liph/liph.h"

int arr[4][4];
int arr_back[4][4];

struct p {
    int x;
    int y;
    p(int x, int y) : x(x), y(y) {}
    bool operator==(p other) { return x == other.x && y == other.y; }
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
    memcpy(&arr_back[0][0], &arr[0][0], 16 * sizeof(int));
}

// clang-format off
std::map<int, int> num2color = {
    {2,     1},
    {4,     2},
    {8,     3},
    {16,    4},
    {32,    5},
    {64,    6},
    {128,   7},
    {256,   8},
    {512,   9},
    {1024,  10},
    {2048,  11},
    {4096,  12},
    {8192,  13},
    {16384,  14},
    {32768,  15},
    {65536,  16}
};
// clang-format on

void show() {
    liph::clear_screen();
    liph::clear_scrollback_buffer();
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
            std::cout << liph::color256bg(num2color[arr[i][j]]) << ss.str() << liph::color_reset();
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
    int arr_tmp[4][4];
    memcpy(&arr_tmp[0][0], &arr[0][0], 16 * sizeof(int));
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
        if (p1 == npos) {
            std::cout << "Game Over!" << std::endl;
            exit(0);
        }
        arr[p1.x][p1.y] = 2;
        memcpy(&arr_back[0][0], &arr_tmp[0][0], 16 * sizeof(int));
    }
    show();
}

void back() {
    memcpy(&arr[0][0], &arr_back[0][0], 16 * sizeof(int));
    show();
}

int main() {
    init();
    show();
    liph::stdin_echo_off();
    liph::stdin_buffering_off();
    while (true) {
        char ch = getchar();
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
        case '0':
            back();
            break;
        default:
            break;
        }
    }
}
