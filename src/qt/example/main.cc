#include <QApplication>
#include <QMainWindow>

#include "2048.h"
#include "ui_mainwindow.h"

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = nullptr) : QMainWindow(parent), ui(new Ui::MainWindow) {
        ui->setupUi(this);

        QObject::connect(ui->exit, &QPushButton::clicked, [this]() { close(); });
        QObject::connect(ui->start_2048, &QPushButton::clicked, [this]() {
            auto w2048 = new Widget2048(this);
            setCentralWidget(w2048);
            setWindowTitle("2048");
        });
    }
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
