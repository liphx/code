#pragma once

#include <QApplication>
#include <QMainWindow>

class MainWindow : public QMainWindow {
public:
    MainWindow(QWidget *parent = nullptr, const QString& title = "");
    ~MainWindow();
    void init();

private:
    QString title;
};

