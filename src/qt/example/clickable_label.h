#pragma once

#include <QLabel>
#include <QWidget>
#include <Qt>

class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    explicit ClickableLabel(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags()) : QLabel(parent) {}
    ~ClickableLabel() {}

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event) { emit clicked(); }
};
