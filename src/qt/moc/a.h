#pragma once

#include <QObject>

class A : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool ok MEMBER ok_)
    Q_CLASSINFO("key1", "value1")
    Q_CLASSINFO("key2", "value2")

public:
    Q_INVOKABLE A(int i = 0, QObject *parent = nullptr) : i_(i), QObject(parent) {}

    int i() const { return i_; }

    void add(int n) {
        i_ += n;
        emit changed();
    }

signals:
    void changed();

private:
    int i_;
    bool ok_;
};
