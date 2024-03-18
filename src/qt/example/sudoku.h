#pragma once

#include <QGridLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <cstdlib>
#include <iostream>
#include <set>
#include <vector>

#include "clickable_label.h"
#include "mainwindow.h"

class Sudoku : public QWidget {
public:
    Sudoku(MainWindow *window_) : QWidget(window_), window(window_) {
        window->setWindowTitle("sudoku");

        layout = new QGridLayout(this);

        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                labels[i][j] = new ClickableLabel(this);
                labels[i][j]->setStyleSheet("border: 1px solid; font-size: 20px;");
                layout->addWidget(labels[i][j], i, j, 1, 1);
                QObject::connect(labels[i][j], &ClickableLabel::clicked, [i, j, this]() {
                    // std::cout << "clicked [" << i << "," << j << "]\n";
                    // std::cout << arr_init[i][j] << '\n';
                    if (arr_init[i][j]) return;
                    delete_selector();
                    selector = new QGridLayout;
                    layout->addLayout(selector, 1, 10, 5, 2);
                    std::set numbers{1, 2, 3, 4, 5, 6, 7, 8, 9};
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
                    int k = 0;
                    for (int number : numbers) {
                        auto label = new ClickableLabel;
                        selector->addWidget(label, k++, 1, 1, 1);
                        label->setText(QString::number(number));
                        label->setStyleSheet("font-size: 20px;");
                        QObject::connect(label, &ClickableLabel::clicked, [i, j, number, this]() {
                            arr[i][j] = number;
                            delete_selector();
                            show();
                        });
                    }
                    auto label = new ClickableLabel;
                    selector->addWidget(label, k, 1, 1, 1);
                    label->setText("clear");
                    label->setStyleSheet("font-size: 20px;");
                    QObject::connect(label, &ClickableLabel::clicked, [i, j, this]() {
                        arr[i][j] = 0;
                        delete_selector();
                        show();
                    });

                    // for (int k1 = 0; k1 < 5; k1++) {
                    //     for (int k2 = 0; k2 < 2; k2++) {
                    //         auto label = new ClickableLabel;
                    //         selector->addWidget(label, k1, k2, 1, 1);
                    //         int number = k1 * 2 + k2 + 1;
                    //         if (number < 10) {
                    //             label->setText(QString::number(number));
                    //         } else {
                    //             label->setText("clear");
                    //             number = 0;
                    //         }
                    //         QObject::connect(label, &ClickableLabel::clicked, [i, j, number, this]() {
                    //             arr[i][j] = number;
                    //             delete_selector();
                    //             show();
                    //         });
                    //     }
                    // }
                });
            }
        }

        auto vlayout = new QVBoxLayout;
        layout->addLayout(vlayout, 7, 10, 2, 2);

        auto restart = new QPushButton(this);
        vlayout->addWidget(restart);
        restart->setText("restart");
        QObject::connect(restart, &QPushButton::clicked, [this]() {
            memcpy(&arr[0][0], &arr_init[0][0], sizeof(arr));
            delete_selector();
            show();
        });

        auto random = new QPushButton(this);
        vlayout->addWidget(random);
        random->setText("random");
        QObject::connect(random, &QPushButton::clicked, [this]() {
            delete_selector();
            init();
        });

        auto exit = new QPushButton(this);
        vlayout->addWidget(exit);
        exit->setText("exit");
        QObject::connect(exit, &QPushButton::clicked, [this]() {
            close();
            window->init();
        });

        init();
    }

    void init() {
        std::srand(time(nullptr));
        memset(&arr[0][0], 0, sizeof(arr));
        while (!GenerateData()) {  // random
            memset(&arr[0][0], 0, sizeof(arr));
        }
        memcpy(&arr_answer[0][0], &arr[0][0], sizeof(arr));
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                std::cout << arr_answer[i][j] << ' ';
            }
            std::cout << std::endl;
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

    bool CanFillIn2(int num, int row, int col) const
    {
        if (num == 0)
            return true;

        for (int i = 1; i <= 9; i++) {
            if (i != col && GetNum(row, i) == num)
                return false;
        }
        for (int i = 1; i <= 9; i++) {
            if (i != row && GetNum(i, col) == num)
                return false;
        }

        int grid_row = 1 + (row - 1) / 3 * 3;
        int grid_col = 1 + (col - 1) / 3 * 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                int x = grid_row + i;
                int y = grid_col + j;
                if (x == row && y == col)
                    continue;
                if (GetNum(x, y) == num)
                    return false;
            }
        }

        return true;
    }

    bool CanFillIn(int num, int row, int col) const {
        if (GetNum(row, col) != 0)
            return false;

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

    void delete_selector() {
        if (!selector) return;
        layout->removeItem(selector);
        for (auto item = selector->takeAt(0); item; item = selector->takeAt(0)) {
            auto label = item->widget();
            label->hide();
            delete label;
        }
        delete selector;
        selector = nullptr;
    }

private:
    MainWindow *window;
    QGridLayout *layout;
    QGridLayout *selector{nullptr};

    int arr[9][9]{};
    int arr_answer[9][9]{};
    int arr_init[9][9]{};
    ClickableLabel *labels[9][9];
};
