#ifndef LTL_DETAIL_TYPE_TRAITS_H_
#define LTL_DETAIL_TYPE_TRAITS_H_

#include <type_traits>

#include "cstddef"

namespace ltl {

template <class T, T v>
struct integral_constant {
    static constexpr T value = v;
    using value_type = T;
    using type = integral_constant<T, v>;
    constexpr operator value_type() const noexcept { return value; }
    constexpr value_type operator()() const noexcept { return value; }
};

template <bool B>
using bool_constant = integral_constant<bool, B>;

using true_type = bool_constant<true>;
using false_type = bool_constant<false>;

template <class T>
struct remove_cv {
    using type = T;
};

template <class T>
struct remove_cv<const T> {
    using type = T;
};

template <class T>
struct remove_cv<volatile T> {
    using type = T;
};

template <class T>
struct remove_cv<const volatile T> {
    using type = T;
};

template <class T>
using remove_cv_t = typename remove_cv<T>::type;

template <class T, class U>
struct is_same : public false_type {};

template <class T>
struct is_same<T, T> : public true_type {};

template <class T>
struct is_void : public is_same<void, remove_cv_t<T>> {};

template <class T>
struct is_null_pointer : public is_same<nullptr_t, remove_cv_t<T>> {};

template <class>
struct is_integral_helper : public false_type {};

template <>
struct is_integral_helper<bool> : public true_type {};

template <>
struct is_integral_helper<char> : public true_type {};

template <>
struct is_integral_helper<signed char> : public true_type {};

template <>
struct is_integral_helper<unsigned char> : public true_type {};

template <>
struct is_integral_helper<short> : public true_type {};

template <>
struct is_integral_helper<unsigned short> : public true_type {};

template <>
struct is_integral_helper<int> : public true_type {};

template <>
struct is_integral_helper<unsigned int> : public true_type {};

template <>
struct is_integral_helper<long> : public true_type {};

template <>
struct is_integral_helper<unsigned long> : public true_type {};

template <>
struct is_integral_helper<long long> : public true_type {};

template <>
struct is_integral_helper<unsigned long long> : public true_type {};

template <class T>
struct is_integral : public is_integral_helper<remove_cv_t<T>>::type {};

template <class T>
struct is_floating_point : public integral_constant<bool, is_same<float, remove_cv_t<T>>::value ||
                                                                  is_same<double, remove_cv_t<T>>::value ||
                                                                  is_same<long double, remove_cv_t<T>>::value> {};

template <class T>
struct is_array : public false_type {};

template <class T>
struct is_array<T[]> : public true_type {};

template <class T, size_t N>
struct is_array<T[N]> : public true_type {};

template <class T>
struct is_pointer_helper : public false_type {};
template <class T>
struct is_pointer_helper<T *> : public true_type {};
template <class T>
struct is_pointer : public is_pointer_helper<remove_cv_t<T>> {};

template <class T>
struct is_lvalue_reference : public false_type {};
template <class T>
struct is_lvalue_reference<T&> : public true_type {};

template <class T>
struct is_rvalue_reference : public false_type {};
template <class T>
struct is_rvalue_reference<T&&> : public true_type {};

using std::is_class;
using std::is_enum;
using std::is_member_function_pointer;
using std::is_member_object_pointer;
using std::is_union;

template <class T>
struct is_function : public bool_constant<!std::is_const<const T>::value> {};

template <class T>
struct is_function<T&> : public false_type {};

template <class T>
struct is_function<T&&> : public false_type {};

template <class T>
struct remove_reference {
    typedef T type;
};

template <class T>
struct remove_reference<T&> {
    typedef T type;
};

template <class T>
struct remove_reference<T&&> {
    typedef T type;
};

template <class T>
using remove_reference_t = typename remove_reference<T>::type;

}  // namespace ltl

#endif  // LTL_DETAIL_TYPE_TRAITS_H_
