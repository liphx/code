#ifndef LIPH_SINGLETON_H_
#define LIPH_SINGLETON_H_

#include <utility>  // std::forward

namespace liph {

template <class T>
class Singleton {
public:
    template <class... Args>
    static T *GetInstance(Args&&...args) {
        static T obj(std::forward<Args>(args)...);
        return &obj;
    }
};

}  // namespace liph

#endif  // LIPH_SINGLETON_H_
