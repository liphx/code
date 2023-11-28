#include <QAction>
#include <QApplication>
#include <QFormLayout>
#include <QLabel>
#include <QLayout>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QPushButton>

class LoginWindow : public QMainWindow {
public:
    LoginWindow(QWidget *parent = 0) : QMainWindow(parent) {
        setWindowTitle("Login");
        QWidget *center = new QWidget;
        this->setCentralWidget(center);

        QFormLayout *layout = new QFormLayout();
        center->setLayout(layout);

        QLineEdit *username_text = new QLineEdit;
        QLineEdit *password_text = new QLineEdit;
        layout->addRow("username", username_text);
        layout->addRow("password", password_text);

        QPushButton *login_button = new QPushButton("login");
        layout->addRow(login_button);
        layout->setLabelAlignment(Qt::AlignLeft);
    }
    ~LoginWindow() {}
};

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = 0) : QMainWindow(parent) {
        // QMenu
        QMenu *menu1 = new QMenu;
        menu1 = menuBar()->addMenu(tr("menu1"));
        QAction *action1 = new QAction(tr("action1"), this);
        menu1->addAction(action1);
    }
    ~MainWindow() {}
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    LoginWindow login;
    login.show();
    return app.exec();
}
