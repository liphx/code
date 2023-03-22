#include <iostream>
using namespace std;

/*
 * 单例模式 singleton
 * 构造函数 private
 * 唯一静态成员对象实例
 */
class Singleton {
private:
    Singleton(int i) : i(i) {}
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    int i;
    static Singleton a;

public:
    static Singleton& instance() { return a; }

    int get() const { return i; }

    void set(int x) { i = x; }
};

Singleton Singleton::a(0);

int main() {
    // Singleton a(0); // private
    // Singleton *a = new Singleton(0); // private

    Singleton& a = Singleton::instance();
    cout << a.get() << endl;  // 0
    a.set(7);
    cout << a.get() << endl;  // 7

    Singleton& b = Singleton::instance();
    cout << b.get() << endl;  // 7

    return 0;
}
