#include "../header.h"

namespace metaprogramming {

// _____________________________________________________________________________
// Recursion

// -----------------------------------------------------------------------------
// With template class
template <int N> struct factorial_template_Class {
  static const int value{N * factorial_template_Class<N - 1>::value};
};

template <> struct factorial_template_Class<1> { static int const value{1}; };

// Helper variable template
template <int N>
constexpr int factorial_template_class_v{factorial_template_Class<N>::value};

// -----------------------------------------------------------------------------
// With template function
template <int N> constexpr int factorial_template_func() {
  return N * factorial_template_func<N - 1>();
}

template <> constexpr int factorial_template_func<1>() { return 1; }

// -----------------------------------------------------------------------------
// With constexpr function
constexpr int factorial_constexpr_func(int i) {
  return (i < 2) ? 1 : i * factorial_constexpr_func(i - 1);
}

// -----------------------------------------------------------------------------

auto recursion() {
  assert(factorial_template_Class<3>::value == 6);
  assert(factorial_template_class_v<3> == 6);
  assert(factorial_template_func<3>() == 6);
  assert(factorial_constexpr_func(3) == 6);
}

// _____________________________________________________________________________
// Selecting between two types

/*
template <bool B, class T, class F>
struct conditional { typedef T type; };

template <class T, class F>
struct conditional<false, T, F> { typedef F type; };

// Helper type
template <bool B, class T, class F>
using conditional_t = typename conditional<B, T, F>::type;
*/

struct Incrementer {
  auto operator()(int x) -> int { return x + 1; }
};

struct Decrementer {
  auto operator()(int x) -> int { return x - 1; }
};

auto conditional_test() {
  std::conditional_t<true, Incrementer, Decrementer> z{};
  assert(z(1) == 2);
}

// _____________________________________________________________________________
// Selecting among several types

template <unsigned N, typename... Cases> struct select;

template <typename T, typename... Cases> struct select<0, T, Cases...> {
  using type = T;
};

template <unsigned N, typename T, typename... Cases>
struct select<N, T, Cases...> : select<N - 1, Cases...> {};

// Helper type
template <unsigned N, typename... Cases>
using select_t = typename select<N, Cases...>::type;

auto selecting_test() {
  select_t<1, Incrementer, Decrementer, int> z{};
  assert(z(1) == 0);
}

// _____________________________________________________________________________
// Enable_if

/*
 template<bool B, class T = void>
 struct enable_if {};

 template<class T>
 struct enable_if <true, T> { typedef T type; };

 // Helper type
 template <bool B, class T = void>
 using enable_if_t = typename enable_if<B, T>::type;
 */

template <typename T> struct StructWithEnable_if {

  StructWithEnable_if<T>() = default;

  template <typename K = T,
            typename = std::enable_if_t<std::is_same_v<K, std::string>, void>>
  StructWithEnable_if<K>(K x){};

  template <typename K = T>
  std::enable_if_t<std::is_same_v<K, std::string>, size_t> length(K k) {
    return k.length();
  };

  template <typename K = T>
  std::enable_if_t<std::is_same_v<K, std::string>> foo() {
    return;
  };
};

auto enable_if_test() {
  {
    StructWithEnable_if<int> x{};
    // StructWithEnable_if<int> y{1}; // Error

    StructWithEnable_if<std::string> z{std::string{"a"}};
    assert(z.length(std::string{"123"}) == 3);
    z.foo();
  }
}

// _____________________________________________________________________________
// Trait
// A trait is used to associate properties with a type

template <typename T> struct value_trait {
  // using value_type = decltype(std::declval<T>().value);
};

template <typename T> using value_trait_v = typename value_trait<T>::value_type;

// Example
struct A {
  int value;
};
template <> struct value_trait<A> {
  using value_type = decltype(std::declval<A>().value);
};

struct B {
  std::string value;
};
template <> struct value_trait<B> {
  using value_type = decltype(std::declval<B>().value);
};

template <typename T> auto set_value(T &x, value_trait_v<T> value) -> void {
  x.value = value;
}

auto trait_test() {
  B b{};
  set_value(b, "Hello");
  assert(b.value == "Hello");
}

// _____________________________________________________________________________

auto run() -> void {
  recursion();
  conditional_test();
  selecting_test();
  enable_if_test();
  trait_test();
}

} // namespace metaprogramming
