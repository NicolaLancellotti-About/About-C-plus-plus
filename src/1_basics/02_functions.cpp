#include "../header.h"
#include <functional>

namespace functions {
// _____________________________________________________________________________
// Pass parameters by lvalue/rvalue reference

auto call_by_lvalue_reference_utility(std::string &x) -> std::string {
  return "by lvalue";
}
auto call_by_rvalue_reference_utility(std::string &&x) -> std::string {
  return "by rvalue";
}

auto call_by_lvalue_reference(std::string &x) -> std::string {
  call_by_lvalue_reference_utility(x);
  // call_by_rvalue_reference_utility(x); // Error
  return "by lvalue";
}

auto call_by_rvalue_reference(std::string &&x) -> std::string {
  call_by_lvalue_reference_utility(x);
  // call_by_rvalue_reference_utility(x); // Error
  return "by rvalue";
}

auto pass_parameters() {
  {
    // By lvalue reference

    std::string string{"Hello, world"};
    assert(call_by_lvalue_reference(string) == "by lvalue");

    //  call_by_lvalue_reference(std::string{"Hello, world"}); // Error
  }

  {
    // By rvalue reference

    assert(call_by_rvalue_reference(std::string{"Hello, world"}) ==
           "by rvalue");

    std::string string{"Hello, world"};
    //    call_by_rvalue_reference(string); // Error

    assert(call_by_rvalue_reference(std::move(string)) == "by rvalue");
  }
}

// _____________________________________________________________________________
// Pass parameters by constant reference

auto concatenate(std::string const &x, std::string const &y) -> std::string {
  return x + y;
}

// _____________________________________________________________________________
// Parameters with default value

auto product(int x, int y = {}) -> int { return x * y; }

// _____________________________________________________________________________
// Inline function

inline auto sum(int x, int y) -> int { return x + y; }

// _____________________________________________________________________________
// constexpr
// A constexpr specifier used in a function or static member variable
// declaration implies inline

constexpr auto constexpr_function(int x) -> int {
  int y{0};
  if (x > 0) {
    y++;
  } else {
    y--;
  }

  for (int i{0}; i < x; ++i) {
    y++;
  }

  switch (x) {
  case 0:
    return 0;
  default:
    return x + y;
  }
}

constexpr double value11{constexpr_function(0)};

// _____________________________________________________________________________
// Lambda

auto lambdas() {

  {
    auto referenceSum{0};      // Capturing value by reference
    auto value{1};             // Capturing value by value
    std::string string{"abc"}; // Capture by move

    std::function<std::string const &(int)> lambda =
        [&sum = referenceSum, value,
         string = std::move(string)](int x) -> std::string const & {
      sum = x + value;
      return string;
    };

    assert(lambda(2) == "abc");
    assert(referenceSum == 3);
  }

  {
    // Capturing all Values by reference or value
    // = -> all by value
    // & -> all by reference
    auto referenceSum{0};
    auto lambda = [&](int value1, int value2) -> int {
      referenceSum = value1 + value2;
      return referenceSum;
    };

    assert(lambda(20, 30) == 50);
    assert(referenceSum == 50);
  }

  {
    // Mix
    auto value10{10};
    auto referenceSum5{0};
    auto lambda5 = [=, &referenceSum5](int value1, int value2) -> int {
      referenceSum5 = value1 + value2 + value10;
      return referenceSum5;
    };
    auto sum5{lambda5(20, 30)};
    assert(referenceSum5 == 60);
    assert(sum5 == 60);
  }

  {
    // mutable
    auto lambda6 = [value = 1](int x) mutable -> int {
      value++;
      return value + x;
    };
    assert(lambda6(1) == 3);
  }

  {
    // function pointer
    int (*functionPointer)(int) noexcept = [](int x) noexcept { return x; };
    assert(functionPointer(12) == 12);
  }
}
// _____________________________________________________________________________

auto run() -> void {
  // Passing parameters by lvalue/rvalue reference
  pass_parameters();

  // Passing parameters by constant reference
  assert(concatenate("a", "b") == "ab");

  // Parameters with default value
  assert(product(1) == 0);
  assert(product(1, 2) == 2);

  // Inline function
  assert(sum(1, 2) == 3);

  // constexpr
  assert(value11 == 0);

  // Lambdas
  lambdas();
}

} // namespace functions
