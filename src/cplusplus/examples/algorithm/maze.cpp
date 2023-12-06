#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <stack>

struct Pos {
    int x;
    int y;
    int di;

    void next() {
        if (di == 0)
            ++y;
        else if (di == 1)
            ++x;
        else if (di == 2)
            --y;
        else  // 3
            --x;
    }
};

typedef int Maze[10][10];

void print(std::stack<Pos> s) {
    std::stack<Pos> t;
    while (!s.empty()) {
        t.push(s.top());
        s.pop();
    }
    std::cout << " [";
    while (!t.empty()) {
        Pos cur = t.top();
        std::cout << "(" << cur.x << "," << cur.y << "," << cur.di << ")";
        t.pop();
    }
    std::cout << "]" << std::endl;
}

void MazePath(Maze maze, Pos start, Pos end) {
    std::stack<Pos> s;
    s.push({start.x, start.y, start.di});
    Maze visited = {{0}};
    while (!s.empty()) {
        Pos cur = s.top();
        if (cur.x == end.x && cur.y == end.y) {
            print(s);
            return;
        }

        cur.next();
        if (maze[cur.x][cur.y] && !visited[cur.x][cur.y]) {
            visited[cur.x][cur.y] = 1;
            s.push({cur.x, cur.y, 0});
        } else if (s.top().di < 3) {
            ++s.top().di;
        } else {
            s.pop();
        }
    }
    std::cout << "No solution" << std::endl;
}

int main() {
    Maze maze = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 1, 1, 0, 1, 1, 1, 0, 1, 0}, {0, 1, 1, 0, 1, 1, 1, 0, 1, 0},
            {0, 1, 1, 1, 1, 0, 0, 1, 1, 0}, {0, 1, 0, 0, 0, 1, 1, 1, 1, 0}, {0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
            {0, 1, 0, 1, 1, 1, 0, 1, 1, 0}, {0, 1, 0, 0, 0, 1, 0, 0, 1, 0}, {0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
    Pos start = {1, 1, 0}, end = {8, 8, 0};
    MazePath(maze, start, end);
}
