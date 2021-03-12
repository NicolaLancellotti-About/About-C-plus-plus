#include "../header.h"
#include <optional>
#include <set>
#include <typeindex>

namespace miscellaneous {

// _____________________________________________________________________________
// References to arrays

template <class T, int N> auto f1(T (&r)[N]) {}

auto f2(int (&r)[10]) {}

auto references_to_arrays() {
  int a10[10];
  int a11[11];
  f1(a10);
  f1(a11);
  f2(a10);
  // f2(a11); // Error
}

// _____________________________________________________________________________
// Attributes

// deprecated
[[deprecated("explain the rationale for deprecation")]] auto
deprecated_func(int &x);

namespace [[deprecated]] deprecated_namespace {};

enum class deprecated_enum_case {
  OK,
  Deprecated [[deprecated]],
};

// noreturn
[[noreturn]] auto unimplemented() { exit(-1); }

// nodiscard
[[nodiscard]] auto nodiscard_function_1() -> int { return 10; }

// function returning an enumeration or class declared nodiscard *by value*
struct [[nodiscard]] NodiscardStruct {};
auto nodiscard_function_2() -> NodiscardStruct { return NodiscardStruct{}; }

auto can_discard_function_3(NodiscardStruct &r) -> NodiscardStruct & {
  return r;
}

// other

auto some_attributes() -> int {
  [[maybe_unused]] int x{10}; // Suppresses warnings on unused entities.
  switch (10) {
  case 1:
    [[fallthrough]];
  case 10:
    return 1;
  default:
    return 0;
  }
}

auto attributes() {
  // nodiscard_function_1(); // Warning
  // nodiscard_function_2(); // Warning
  NodiscardStruct value{};
  can_discard_function_3(value);
}

// _____________________________________________________________________________
// Type Identification

auto type_identification() {
  std::type_info const &type_info_string_1{typeid(std::string)};
  std::type_info const &type_info_string_2{typeid(std::string{"Hello"})};
  std::set<std::type_index> type_index{type_info_string_1};
  assert(type_info_string_1 == type_info_string_2);

  assert(std::string{typeid(bool).name()} == "b");
}

// _____________________________________________________________________________
// Optionals

auto foo(bool flag) -> std::optional<std::string> {
  if (flag) {
    return std::string{"Hello"};
  }
  return std::nullopt;
}

auto optional() {
  if (auto x{foo(true)}) {
    assert(x == "Hello");
  } else {
    assert(false);
  }

  if (auto x{foo(false)}) {
    assert(false);
  } else {
    assert(true);
  }
}

// _____________________________________________________________________________

auto run() -> void {
  references_to_arrays();
  attributes();
  type_identification();
  optional();

  {
    // Alias
    using Char = char;
    using Uchar = unsigned char;
    // using Uchar2 = unsigned Char; // you cannot apply type specifiers to an
    // alias
  }

  {
    // Range-based for loop
    int array[2]{1, 2};
    int i{1};
    for (auto v : array) {
      assert(v == i++);
    }
  }

  {
    // Compile time features

    // Compile-time static if
    if constexpr (1) {
    }

    // static assert
    static_assert(2 == 2, "Error");
  }
}

} // namespace miscellaneous
