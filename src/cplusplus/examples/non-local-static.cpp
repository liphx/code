/* // A.h */
/* #ifndef A_H_ */
/* #define A_H_ */

/* #include <iostream> */

/* class A { */
/* public: */
/*     int n; */
/*     A(int i) : n(i) { std::cout << "A(" << n << ")" << std::endl; } */
/*     int f() { return n; } */
/* }; */

/* #endif */

/* // A.cpp */
/* #include "A.h" */

/* extern A a; */
/* A a(100); */

/* // B.cpp */
/* #include "A.h" */

/* extern A a; */

/* class B { */
/* public: */
/*     B() { */
/*         std::cout << "B()" << std::endl; */
/*         std::cout << a.f() << std::endl; */
/*     } */
/* }; */

/* B b; */

/* // 不同翻译单元内 non-local static 对象初始化的顺序是不确定的 */
/* // g++ A.cpp B.cpp */
/* // g++ B.cpp A.cpp */
/* // 使用 Singleton 解决此类问题 */
/* // A& GetA() { */
/* //     static A a(100); */
/* //     return a; */
/* // } */
int main() {}
