#include <iostream>
using namespace std;

class Shape {
public:
    virtual void f() = 0;
    static Shape *factory(const string& type);
    virtual ~Shape() {}
};

class Circle : public Shape {
    Circle() {}
    friend class Shape;

public:
    void f() { cout << "circle" << endl; }
};

class Square : public Shape {
    Square() {}
    friend class Shape;

public:
    void f() { cout << "square" << endl; }
};

Shape *Shape::factory(const string& type) {
    if (type == "Circle") return new Circle;
    if (type == "Square") return new Square;
    return nullptr;
}

int main() {
    Shape *s1 = Shape::factory("Circle");
    Shape *s2 = Shape::factory("Square");
    if (s1) s1->f();  // circle
    if (s2) s2->f();  // square

    delete s1;
    delete s2;
}
