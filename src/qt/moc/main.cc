#include <QDebug>
#include <QMetaClassInfo>
#include <QString>

#include "a.h"

int main() {
    A *a = new A;
    auto *meta = a->metaObject();
    qDebug() << meta->className();  // A
    for (int i = 0; i < meta->classInfoCount(); i++) {
        auto info = meta->classInfo(i);
        qDebug() << info.name() << info.value();
    }

    QObject *obj = meta->newInstance(42);
    // qobject_cast doesn't use RTTI
    A *a2 = qobject_cast<A *>(obj);
    qDebug() << a2->i();                  // 42
    qDebug() << a2->inherits("QObject");  // true

    a2->setProperty("ok", true);
    qDebug() << a2->property("ok");
    a2->setProperty("error", "success");
    qDebug() << a2->property("error");

    QObject::connect(a2, &A::changed, [] { qDebug() << "slot"; });
    a2->add(10);
}
