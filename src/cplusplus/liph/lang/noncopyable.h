#ifndef LIPH_LANG_NONCOPYABLE_H_
#define LIPH_LANG_NONCOPYABLE_H_

// from boost::noncopyable
// https://www.boost.org/users/license.html

namespace liph {

//  Private copy constructor and copy assignment ensure classes derived from
//  class noncopyable cannot be copied.

namespace noncopyable_ {  // protection from unintended ADL

#ifndef BOOST_NONCOPYABLE_BASE_TOKEN_DEFINED
#define BOOST_NONCOPYABLE_BASE_TOKEN_DEFINED

// noncopyable derives from base_token to enable Type Traits to detect
// whether a type derives from noncopyable without needing the definition
// of noncopyable itself.
//
// The definition of base_token is macro-guarded so that Type Traits can
// define it locally without including this header, to avoid a dependency
// on Core.

struct base_token {};

#endif  // #ifndef BOOST_NONCOPYABLE_BASE_TOKEN_DEFINED

// class noncopyable : base_token {
class noncopyable {
protected:
    constexpr noncopyable() = default;
    ~noncopyable() = default;

    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
};

}  // namespace noncopyable_

typedef noncopyable_::noncopyable noncopyable;

}  // namespace liph

#endif  // LIPH_LANG_NONCOPYABLE_H_
