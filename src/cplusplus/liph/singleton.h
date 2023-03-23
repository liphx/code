#ifndef LIPH_SINGLETON_H_
#define LIPH_SINGLETON_H_

#include <utility>  // std::forward

namespace liph {

template <class T>
class singleton {
public:
    template <class... Args>
    static T *instance(Args&&...args) {
        static T obj(std::forward<Args>(args)...);
        return &obj;
    }
};

}  // namespace liph

#endif  // LIPH_SINGLETON_H_
