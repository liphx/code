#ifndef LTL_DETAIL_CONSTRUCT_H_
#define LTL_DETAIL_CONSTRUCT_H_

#include "utility"

namespace ltl {

template <class T, class... Args>
constexpr T *construct_at(T *p, Args&&...args) {
    return ::new (const_cast<void *>(static_cast<const volatile void *>(p))) T(forward<Args>(args)...);
}

template <class T>
constexpr void destroy_at(T *p) {
    p->~T();
}

}  // namespace ltl

#endif  // LTL_DETAIL_CONSTRUCT_H_
