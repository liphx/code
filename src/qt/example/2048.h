#pragma once

#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <cstdlib>
#include <iostream>
#include <vector>

#include "mainwindow.h"

class Widget2048 : public QWidget {
public:
    Widget2048(MainWindow *window_) : QWidget(window_), window(window_) {
        window->setWindowTitle("2048");

        auto layout = new QGridLayout(this);

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                labels[i][j] = new QLabel(this);
                labels[i][j]->setStyleSheet("border: 1px solid;");
                layout->addWidget(labels[i][j], i, j, 1, 1);
            }
        }

        auto vlayout = new QVBoxLayout;
        layout->addLayout(vlayout, 3, 4, 1, 1);

        auto back = new QPushButton(this);
        vlayout->addWidget(back);
        back->setText("back");
        QObject::connect(back, &QPushButton::clicked, [this]() {
            memcpy(&arr[0][0], &arr_back[0][0], sizeof(arr));
            show();
        });

        auto restart = new QPushButton(this);
        vlayout->addWidget(restart);
        restart->setText("restart");
        QObject::connect(restart, &QPushButton::clicked, [this]() { init(); });

        auto exit = new QPushButton(this);
        vlayout->addWidget(exit);
        exit->setText("exit");
        QObject::connect(exit, &QPushButton::clicked, [this]() {
            close();
            window->init();
        });

        setFocusPolicy(Qt::StrongFocus);

        init();
    }

    ~Widget2048() {}

    void init() {
        memset(&arr[0][0], 0, sizeof(arr));
        std::srand(time(nullptr));
        p p1 = new2();
        arr[p1.x][p1.y] = 2;
        p p2 = new2();
        arr[p2.x][p2.y] = 2;
        memcpy(&arr_back[0][0], &arr[0][0], sizeof(arr));
        show();
    }

    void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_Left) {
            move(1);
        } else if (event->key() == Qt::Key_Right) {
            move(2);
        } else if (event->key() == Qt::Key_Up) {
            move(3);
        } else if (event->key() == Qt::Key_Down) {
            move(4);
        }
    }

private:
    void show() {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                if (arr[i][j]) {
                    labels[i][j]->setText(QString::number(arr[i][j]));
                } else {
                    labels[i][j]->clear();
                }
            }
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

    struct p {
        int x;
        int y;
        p(int x, int y) : x(x), y(y) {}
        bool operator==(p other) { return x == other.x && y == other.y; }
    };

    void move(int di) {
        int arr_tmp[4][4];
        memcpy(&arr_tmp[0][0], &arr[0][0], sizeof(arr));
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
                // Game Over
                close();
            }
            arr[p1.x][p1.y] = 2;
            memcpy(&arr_back[0][0], &arr_tmp[0][0], sizeof(arr));
        }
        show();
    }

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
        return free[std::rand() % free.size()];
    }

private:
    int arr[4][4]{};
    int arr_back[4][4]{};
    static inline const p npos = {-1, -1};

    MainWindow *window;
    QLabel *labels[4][4];
};
