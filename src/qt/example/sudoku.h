#pragma once

#include <QDebug>
#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <cstdlib>
#include <set>
#include <vector>

#include "clickable_label.h"
#include "mainwindow.h"

class Sudoku : public QWidget {
public:
    Sudoku(MainWindow *window_) : QWidget(window_), window(window_) {
        setui();

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                QObject::connect(labels[i][j], &ClickableLabel::clicked, [i, j, this]() {
                    qDebug("clicked [%d, %d]: %d", i, j, arr_init[i][j]);
                    if (arr_init[i][j]) return;
                    clear_selector();
                    std::set numbers{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
                    for (int k = 0; k < 9; k++) {
                        if (k != j && arr[i][k]) {
                            numbers.erase(arr[i][k]);
                        }
                    }
                    for (int k = 0; k < 9; k++) {
                        if (k != i && arr[k][j]) {
                            numbers.erase(arr[k][j]);
                        }
                    }
                    for (int number : numbers) {
                        auto label = new ClickableLabel;
                        selector->addWidget(label);
                        if (number) {
                            label->setText(QString::number(number));
                        } else {
                            label->setText("clear");
                        }
                        auto font = label->font();
                        font.setPointSize(14);
                        label->setFont(font);
                        label->setStyleSheet("border: 1px solid;");
                        label->setFixedSize(100, 50);
                        QObject::connect(label, &ClickableLabel::clicked, [i, j, number, this]() {
                            arr[i][j] = number;
                            clear_selector();
                            show();
                        });
                    }
                    selector->addStretch();
                    selector->setSpacing(0);
                });
            }
        }

        init();
    }

    void setui() {
        window->setWindowTitle("sudoku");

        // main layout: 16:9
        auto layout = new QGridLayout(this);
        layout->setRowStretch(0, 1);
        layout->setRowStretch(1, 7);
        layout->setRowStretch(2, 1);
        layout->setColumnStretch(0, 9);
        layout->setColumnStretch(1, 14);
        layout->setColumnStretch(2, 9);

        QGridLayout *grid = new QGridLayout;
        layout->addLayout(grid, 1, 1);
        grid->setHorizontalSpacing(15);
        grid->setVerticalSpacing(15);
        QGridLayout *grids[9];
        for (int i = 0; i < 9; i++) {
            grids[i] = new QGridLayout;
            grids[i]->setHorizontalSpacing(0);
            grids[i]->setVerticalSpacing(0);
            grid->addLayout(grids[i], i / 3, i % 3);
        }

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                labels[i][j] = new ClickableLabel(this);
                labels[i][j]->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                labels[i][j]->setStyleSheet("border: 1px solid;");
                auto font = labels[i][j]->font();
                font.setPointSize(16);
                labels[i][j]->setFont(font);
                grids[(i / 3) * 3 + j / 3]->addWidget(labels[i][j], i % 3, j % 3);
            }
        }

        auto select = new QGridLayout;
        layout->addLayout(select, 1, 2);
        select->setRowStretch(0, 2);
        select->setRowStretch(1, 1);
        select->setColumnStretch(0, 1);
        select->setColumnStretch(1, 3);
        select->setColumnStretch(2, 3);

        selector = new QVBoxLayout;
        select->addLayout(selector, 0, 1);

        auto vlayout = new QVBoxLayout;
        select->addLayout(vlayout, 1, 1);
        vlayout->addStretch();
        auto restart = new QPushButton(this);
        vlayout->addWidget(restart);
        restart->setText("restart");
        restart->setFixedSize(100, 50);
        QObject::connect(restart, &QPushButton::clicked, [this]() {
            memcpy(&arr[0][0], &arr_init[0][0], sizeof(arr));
            clear_selector();
            show();
        });

        auto random = new QPushButton(this);
        vlayout->addWidget(random);
        random->setText("random");
        random->setFixedSize(100, 50);
        QObject::connect(random, &QPushButton::clicked, [this]() {
            clear_selector();
            init();
        });

        auto exit = new QPushButton(this);
        vlayout->addWidget(exit);
        exit->setText("exit");
        exit->setFixedSize(100, 50);
        QObject::connect(exit, &QPushButton::clicked, [this]() {
            close();
            window->init();
        });
    }

    void init() {
        std::srand(time(nullptr));
        memset(&arr[0][0], 0, sizeof(arr));
        while (!GenerateData()) {  // random
            memset(&arr[0][0], 0, sizeof(arr));
        }
        memcpy(&arr_answer[0][0], &arr[0][0], sizeof(arr));
        auto dbg = qDebug();
        dbg << "answer:\n";
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                dbg << arr_answer[i][j];
            }
            dbg << '\n';
        }
        GenerateProblem();
        memcpy(&arr_init[0][0], &arr[0][0], sizeof(arr));
        show();
    }

    bool success() {
        int unknown = 81;
        for (const auto& row : arr) unknown -= std::count(std::begin(row), std::end(row), 0);
        return unknown == 0;
    }

    // if can be solved by one approach
    bool IsUnique() { return true; }

    void SetUnknownIfUnique(int row, int col) {
        int origin = GetNum(row, col);
        if (origin == 0) return;
        SetNum(0, row, col);
        if (!IsUnique()) SetNum(origin, row, col);
    }

    void GenerateProblem() {
        // Randomly set one for each row, col, grid as unknown
        // repeat
        static const int grid_begin[9][2] = {{1, 1}, {1, 4}, {1, 7}, {4, 1}, {4, 4}, {4, 7}, {7, 1}, {7, 4}, {7, 7}};
        for (int times = 2; times > 0; times--) {
            for (int row = 1; row <= 9; row++) SetUnknownIfUnique(row, rand() % 9 + 1);
            for (int col = 1; col <= 9; col++) SetUnknownIfUnique(rand() % 9 + 1, col);
            for (int grid = 1; grid <= 9; grid++) {
                int row = grid_begin[grid - 1][0] + rand() % 3;
                int col = grid_begin[grid - 1][1] + rand() % 3;
                SetUnknownIfUnique(row, col);
            }
        }
    }

    void SetNum(int num, int row, int col) { arr[row - 1][col - 1] = num; }

    int GetNum(int row, int col) const { return arr[row - 1][col - 1]; }

    int RandomNum(const std::vector<int>& nums) const {
        if (nums.empty()) return -1;
        return nums[std::rand() % nums.size()];
    }

    bool GenerateData() {
        for (int num = 1; num <= 9; num++) {
            for (int row = 1; row <= 9; row++) {
                std::vector<int> nums;
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

    bool CanFillIn2(int num, int row, int col) const {
        if (num == 0) return true;

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

    ~Sudoku() {}

    void show() {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (arr[i][j]) {
                    labels[i][j]->setText(QString::number(arr[i][j]));
                } else {
                    labels[i][j]->clear();
                }
            }
        }
        check();
    }

    void check() {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (arr_init[i][j] || arr[i][j] == 0 || CanFillIn2(arr[i][j], i + 1, j + 1)) {
                    labels[i][j]->setStyleSheet("border: 1px solid;");
                    continue;
                }
                labels[i][j]->setStyleSheet("background-color: red;");
                return;
            }
        }
    }

    void clear_selector() {
        for (auto item = selector->takeAt(0); item; item = selector->takeAt(0)) {
            auto label = item->widget();
            if (label) {
                label->hide();
                delete label;
            }
        }
    }

private:
    // ui
    MainWindow *window;
    QVBoxLayout *selector;
    ClickableLabel *labels[9][9];

    // data
    int arr[9][9]{};
    int arr_answer[9][9]{};
    int arr_init[9][9]{};
};
