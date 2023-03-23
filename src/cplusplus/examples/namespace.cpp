/*
# 名字控制

static基本含义
1、在固定的地址上进行存储分配，也就是说对象是在一个特殊的静态数据区上创建的，而不是每次函数调用时在堆栈上创建的
2、对一个特定的翻译单元来说是局部的，控制名字的可见性，所以这个名字在单元或类之外是不可见的

如果没有为一个内建类型的静态变量提供一个初始值的话，编译器会确保在程序开始时它被初始化为0
无论何时设计一个包含static变量的函数时，都应该记住多线程问题

在定义一个静态对象时如果没有制定构造函数参数，这个类就必须有默认的构造函数

如果一个包含static对象的函数从未被调用，那这个对象的构造函数也不会调用

当退出main()时，被创建的对象的析构函数按创建时相反的顺序调用
1-10-static.cpp
```cpp
#include <iostream>
using namespace std;

class A{
    static int i;
    int x;
    const static int n = 10;
public:
    A(int x) :x(x) { cout << "A::构造函数 --- " << x << endl; }
    ~A() { cout << "A::析构函数 --- " << x << endl; }
};

int A::i = 1;

static A a1(1);

void f(){ static A a2(2); }
void g(){ static A a3(3); }

int main()
{
    static A a4(4);
    g();
    f();
    return 0;
}
```

所有的全局对象都是隐含为静态储存的
int a = 0; 等价于 extern int a = 0;
static int a = 0;只改变了a的可见性，但不管是static还是extern的全局变量，都是驻留在静态数据区
局部变量的static的可见性是不会改变的，因为这时extern是没有意义的

名字空间 namespace
只能在全局范围内定义，但可以互相嵌套
定义结尾不必加;号
每个翻译单元可以有一个未命名的namespace,把局部名字放在未命名的namespace中，不需要static就可以让它作为内部连接
可以在namespace的类定义中插入一个友元，这样友元函数就成了namespace的一个成员
使用namespace，使用using指令或::
不要把一个全局的using指令引入到一个头文件

类的静态成员
静态成员变量 定义必须在类的外部
class A{
    static int i;//private
public:
    //...
}
int A::i = 1;

不能在局部类(在函数中定义的类)中有静态成员变量

静态成员函数没有this,比一般的成员函数访问速度快一点

在一指定的翻译单元中，静态对象的初始化顺序严格按照对象在该单元中定义出现的顺序
但在不同翻译单元则无法指定顺序

替代连接说明
在C++中使用C库函数 extern "C" float(int a,char b);
告诉编译器f()是C连接，这样就不会转换函数名
*/

int main() {}
