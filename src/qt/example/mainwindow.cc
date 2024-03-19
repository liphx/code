#include "mainwindow.h"

#include "2048.h"
#include "sudoku.h"

MainWindow::MainWindow(QWidget *parent, const QString& name) : QMainWindow(parent), title(name) {
    resize(1920, 1080);
    init();
}

void MainWindow::init() {
    setWindowTitle(title);
    auto body = new QWidget(this);
    setCentralWidget(body);
    auto layout = new QVBoxLayout;
    body->setLayout(layout);
    auto start_2048 = new QPushButton;
    auto sudoku = new QPushButton;
    auto exit = new QPushButton;
    layout->addStretch();
    layout->addWidget(start_2048);
    layout->addWidget(sudoku);
    layout->addWidget(exit);
    layout->addStretch();
    layout->setSpacing(0);
    start_2048->setFixedSize(120, 60);
    sudoku->setFixedSize(120, 60);
    exit->setFixedSize(120, 60);
    start_2048->setText("2048");
    sudoku->setText("sudoku");
    exit->setText("exit");
    QObject::connect(start_2048, &QPushButton::clicked, [this]() { setCentralWidget(new Widget2048(this)); });
    QObject::connect(sudoku, &QPushButton::clicked, [this]() { setCentralWidget(new Sudoku(this)); });
    QObject::connect(exit, &QPushButton::clicked, [this]() { close(); });
}

MainWindow::~MainWindow() {}
