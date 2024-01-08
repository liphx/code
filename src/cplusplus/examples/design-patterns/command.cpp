#include <iostream>
#include <vector>

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
