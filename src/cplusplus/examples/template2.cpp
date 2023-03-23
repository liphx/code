/* template <class T> */
/* class A { */
/* public: */
/*     void f() {} */
/* }; */

/* /1* template<> class A<int> {}; *1/ */

/* template <class T> */
/* class B : public A<T> { */
/* public: */
/*     void g() { */
/*         /1* f(); *1/ */
/*         this->f(); */
/*     } */

/*     void h() { */
/*         /1* f(); *1/ */
/*         using A<T>::f; */
/*         f(); */
/*     } */

/*     void i() { */
/*         /1* f(); *1/ */
/*         A<T>::f(); */
/*     } */
/* }; */

/* int main() { */
/*     B<int> b; */
/*     b.f(); */
/* } */

int main() {}
