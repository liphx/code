#include "type_traits"

#include <array>

#include "gtest/gtest.h"

namespace type_traits_test {

struct version : public ltl::integral_constant<int, 100> {};

TEST(type_traits, integral_constant) {
    EXPECT_EQ(version(), 100);
    version value;
    EXPECT_EQ(value, 100);

    EXPECT_TRUE(ltl::true_type());
    EXPECT_FALSE(ltl::false_type());
}

class A {};
enum E : int {};
enum E2 {};
enum class Ec : int {};

template <class T>
T f(T i) {
    static_assert(ltl::is_integral<T>::value, "Integral required.");
    return i;
}

TEST(type_traits, type_categories) {
    // is_void
    EXPECT_TRUE(ltl::is_void<void>::value);
    EXPECT_TRUE(ltl::is_void<const void>::value);
    EXPECT_TRUE(ltl::is_void<volatile void>::value);
    EXPECT_TRUE(ltl::is_void<const volatile void>::value);
    EXPECT_FALSE(ltl::is_void<int>::value);

    // is_null_pointer, is_pointer
    EXPECT_TRUE(ltl::is_null_pointer<decltype(nullptr)>::value);
    EXPECT_FALSE(ltl::is_null_pointer<int *>::value);
    EXPECT_FALSE(ltl::is_pointer<decltype(nullptr)>::value);
    EXPECT_TRUE(ltl::is_pointer<int *>::value);

    // is_integral
    EXPECT_FALSE(ltl::is_integral<A>::value);
    EXPECT_FALSE(ltl::is_integral<E>::value);
    EXPECT_FALSE(ltl::is_integral<float>::value);
    EXPECT_TRUE(ltl::is_integral<int>::value);
    EXPECT_TRUE(ltl::is_integral<bool>::value);
    EXPECT_EQ(f(100), 100);
    // f(100.0); // compile error

    // is_floating_point
    EXPECT_FALSE(ltl::is_floating_point<A>::value);
    EXPECT_TRUE(ltl::is_floating_point<float>::value);
    EXPECT_FALSE(ltl::is_floating_point<float&>::value);
    EXPECT_TRUE(ltl::is_floating_point<double>::value);
    EXPECT_FALSE(ltl::is_floating_point<double&>::value);
    EXPECT_FALSE(ltl::is_floating_point<int>::value);

    // is_array
    EXPECT_FALSE(ltl::is_array<A>::value);
    EXPECT_TRUE(ltl::is_array<A[]>::value);
    EXPECT_TRUE(ltl::is_array<A[3]>::value);
    EXPECT_FALSE(ltl::is_array<float>::value);
    EXPECT_FALSE(ltl::is_array<int>::value);
    EXPECT_TRUE(ltl::is_array<int[]>::value);
    EXPECT_TRUE(ltl::is_array<int[3]>::value);
    using array_int_3 = std::array<int, 3>;
    EXPECT_FALSE(ltl::is_array<array_int_3>::value);

    // is_lvalue_reference, is_rvalue_reference
    EXPECT_FALSE(ltl::is_lvalue_reference<A>::value);
    EXPECT_TRUE(ltl::is_lvalue_reference<A&>::value);
    EXPECT_FALSE(ltl::is_lvalue_reference<A&&>::value);
    EXPECT_FALSE(ltl::is_lvalue_reference<int>::value);
    EXPECT_TRUE(ltl::is_lvalue_reference<int&>::value);
    EXPECT_FALSE(ltl::is_lvalue_reference<int&&>::value);

    EXPECT_FALSE(ltl::is_rvalue_reference<A>::value);
    EXPECT_FALSE(ltl::is_rvalue_reference<A&>::value);
    EXPECT_TRUE(ltl::is_rvalue_reference<A&&>::value);
    EXPECT_FALSE(ltl::is_rvalue_reference<int>::value);
    EXPECT_FALSE(ltl::is_rvalue_reference<int&>::value);
    EXPECT_TRUE(ltl::is_rvalue_reference<int&&>::value);

    // is_enum
    EXPECT_FALSE(ltl::is_enum<A>::value);
    EXPECT_TRUE(ltl::is_enum<E2>::value);
    EXPECT_TRUE(ltl::is_enum<Ec>::value);
    EXPECT_FALSE(ltl::is_enum<int>::value);
}

}  // namespace type_traits_test
