#ifndef LIPH_LANG_MAKE_DEFAULT_H_
#define LIPH_LANG_MAKE_DEFAULT_H_

namespace liph {

template <class T>
T make_default() {
    return T();
}

}  // namespace liph

#endif  // LIPH_LANG_MAKE_DEFAULT_H_
