#include "liph/liph.h"
using namespace liph;

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
    return free[::rand() % free.size()];
}

int score() {
    int *p = &arr[0][0];
    return std::accumulate(p, p + 16, 0, [](int total, int x) { return x > 2 ? total + x : total; });
}

std::map<int, color256> num2color = {{2, color256::Green}, {4, color256::Blue}, {8, color256::DarkGreen},
        {16, color256::DarkBlue}, {32, color256::Red}, {64, color256::DarkRed}, {128, color256::Yellow},
        {256, color256::Purple}, {512, color256::Maroon}, {1024, color256::Teal}, {2048, color256::GreenYellow},
        {4096, color256::DarkOrange}, {8192, color256::HotPink}, {16384, color256::DarkMagenta},
        {32768, color256::DarkCyan}, {65536, color256::Grey}};

void show() {
    clear_screen();
    clear_scrollback_buffer();
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
            std::cout << color256bg(num2color[arr[i][j]]) << ss.str() << color_reset();
        }
        std::cout << std::setw(4) << std::endl;
    }
}

void init() {
    stdin_echo_off();
    stdin_buffering_off();
    std::cout << color256bg(color256::Black);
    std::cout << color256fg(color256::White);

    ::srand(0);
    p p1 = new2();
    arr[p1.x][p1.y] = 2;
    p p2 = new2();
    arr[p2.x][p2.y] = 2;
    memcpy(&arr_back[0][0], &arr[0][0], 16 * sizeof(int));

    show();
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

void loop() {
    char ch;
    while (true) {
        std::cin.get(ch);
        if (ch == 4) {  // EOT
            break;
        }
        switch (ch) {
        case 'A':
        case 'a':
            move(1);
            break;
        case 'D':
        case 'd':
            move(2);
            break;
        case 'W':
        case 'w':
            move(3);
            break;
        case 'S':
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
    reset_screen();
}

int main() {
    init();
    loop();
}
