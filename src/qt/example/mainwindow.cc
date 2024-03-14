#include "mainwindow.h"

#include "2048.h"
#include "sudoku.h"

MainWindow::MainWindow(QWidget *parent, const QString& name) : QMainWindow(parent), title(name) {
    resize(800, 450);
    init();
}

void MainWindow::init() {
    auto body = new QWidget(this);
    setCentralWidget(body);
    setWindowTitle(title);

    auto start_2048 = new QPushButton(body);
    start_2048->setText("2048");
    start_2048->setGeometry(QRect(560, 110, 120, 60));
    QObject::connect(start_2048, &QPushButton::clicked, [this]() { setCentralWidget(new Widget2048(this)); });

    auto sudoku = new QPushButton(body);
    sudoku->setText("sudoku");
    sudoku->setGeometry(QRect(560, 190, 120, 60));
    QObject::connect(sudoku, &QPushButton::clicked, [this]() { setCentralWidget(new Sudoku(this)); });

    auto exit = new QPushButton(body);
    exit->setText("exit");
    exit->setGeometry(QRect(560, 270, 120, 60));
    QObject::connect(exit, &QPushButton::clicked, [this]() { close(); });
}

MainWindow::~MainWindow() {}
