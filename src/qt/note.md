Qt Account
lipenghua@lipenghua.com

qt 6.6.2

book: https://book.douban.com/subject/30320649/

Meta-Object System(MOC)

```cpp
QMetaObject::Connection QObject::connect(const QObject *sender,
                                         const char *signal,
                                         const QObject *receiver,
                                         const char *method,
                                         Qt::ConnectionType type = Qt::AutoConnection);

QMetaObject::Connection QObject::connect(const QObject *sender,
                                         const QMetaMethod& signal,
                                         const QObject *receiver,
                                         const QMetaMethod& method,
                                         Qt::ConnectionType type = Qt::AutoConnection);
```
