#include <iostream>
using namespace std;

int main() {
    // while (char c = cin.get() != 'q') {
    //     cout << c << endl;
    // }
    // 由于!= 优先级高于 = ，上面代码每次都会打印一个不可打印字符

    // while ((char c = cin.get()) != 'q') {
    //    cout << c << endl;
    // }
    // while语法限制这种用法，编译报错

    char c;
    while ((c = cin.get()) != 'q') {
        cout << c;
    }
}
