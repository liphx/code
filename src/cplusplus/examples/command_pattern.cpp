#include <iostream>
#include <vector>

/*
 * 命令模式
 * 通过将函数封装为对象就能够以参数的形式将其传递给其他函数或对象
 * 告诉它们在履行请求的过程中执行特定的操作
 * 用来消除与事件处理的耦合
 */
class Command {
public:
    virtual void execute() = 0;
    virtual ~Command() {}
};

class Hello : public Command {
public:
    void execute() { std::cout << "hello, "; }
};

class World : public Command {
public:
    void execute() { std::cout << "world." << std::endl; }
};

class A {
    std::vector<Command *> commands;

public:
    void add(Command *c) { commands.emplace_back(c); }
    void run() {
        auto it = commands.begin();
        while (it != commands.end()) {
            (*it++)->execute();
        }
    }
};

int main() {
    Command *hello = new Hello;
    Command *world = new World;
    A a;
    a.add(hello);
    a.add(world);
    a.run();
    delete hello;
    delete world;
}
