#ifndef LIPH_LANG_SINGLETON_H_
#define LIPH_LANG_SINGLETON_H_

// #include <utility>  // std::forward
#include "liph/lang/make_default.h"

namespace liph {

template <class T>
struct singleton {
    // template <class... Args>
    // static T& instance(Args&&...args) {
    //     static T obj(std::forward<Args>(args)...);
    //     return obj;
    // }

    static T& instance() {
        static T obj = make_default<T>();
        return obj;
    }
};

}  // namespace liph

#endif  // LIPH_LANG_SINGLETON_H_
