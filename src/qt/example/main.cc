#include <QApplication>
#include <QMainWindow>

#include "ui_mainwindow.h"

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent), ui(new Ui::MainWindow) { ui->setupUi(this); }
    ~MainWindow() { delete ui; }

private:
    Ui::MainWindow *ui;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec();
}
