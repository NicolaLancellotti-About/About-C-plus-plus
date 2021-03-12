#include "../header.h"
#include <complex>
#include <sstream>

/*
 Overloadable operators

 +     −     ∗     /       %       ˆ         &
 |     ˜     !     =       <       >         +=
 −=    ∗=    /=    %=      ˆ=      &=        |=
 <<    >>    >>=   <<=     ==      !=        <=
 >=    &&    ||    ++      −−      −>∗       ,
 −>    []    ()    new     new[]   delete    delete[]

 */

namespace operators {

// _____________________________________________________________________________
// Function object

class Incrementer {
  int _value;

public:
  Incrementer(int value) : _value{value} {}

  // Call operator also known as the application operator
  auto operator()(int x) const -> int { return x + _value; }
};

auto function_object() {
  auto incrementer{Incrementer{2}};
  assert(incrementer(10) == 12);
}

// _____________________________________________________________________________
// Conversion operators & user-definde literals

class Meter {
  int _value{};

public:
  explicit constexpr Meter(int value) : _value{value} {};

  operator int() const { return _value; }

  operator std::string() const { return std::to_string(_value) + "m"; }
};

constexpr auto operator""_m(unsigned long long m) -> Meter {
  if (m > std::numeric_limits<int>::max()) {
    throw std::invalid_argument("");
  }
  return Meter{static_cast<int>(m)};
}

auto litterals() {
  {
    Meter meter{1};
    meter = 1_m; // litteral type

    int as_int{meter};
    assert(as_int == 1);

    std::string as_string{static_cast<std::string>(meter)};
    assert(as_string == "1m");

    auto throwed{false};
    try {
      Meter x{100000000000000_m};
      assert(x == 0);
    } catch (std::invalid_argument) {
      throwed = true;
    }
    assert(throwed);
  }

  {
    using namespace std::literals;

    auto hello{"hello world"s};
    assert(hello == "hello world");

    auto z{2.0 + 1i};
    assert(std::complex<double>(2.0, 1.0) == z);
  }
}

// _____________________________________________________________________________
// Operators as members functions

class Vector {

public:
  int x, y;

  Vector(int x, int y) : x{x}, y{y} {}

  //  Operators as members functions (needs access to representation)
  auto operator&() -> Vector & = delete;

  auto operator=(Vector const &rhs) -> Vector & {
    x = rhs.x;
    y = rhs.y;
    return *this;
  }

  auto operator+=(Vector const &rhs) -> Vector & {
    x += rhs.x;
    y += rhs.y;
    return *this;
  }

  // prefix
  auto operator++() -> Vector & {
    x += 1;
    y += 1;
    return *this;
  }

  // postfix (dummy argument)
  auto operator++(int) -> Vector {
    auto old{*this};
    x += 1;
    y += 1;
    return old;
  }

  auto operator[](int index) const -> int {
    switch (index) {
    case 0:
      return x;
    case 1:
      return y;
    default:
      throw std::out_of_range("");
    }
  }

  auto operator[](int index) -> int & {
    switch (index) {
    case 0:
      return x;
    case 1:
      return y;
    default:
      throw std::out_of_range("");
    }
  }
};

auto member_operators() {
  {
    Vector v1{3, 1};
    Vector v2{v1};
    assert(v2.x == 3 && v2.y == 1);
  }

  {
    Vector v1{3, 1};
    v1 += {1, 2};
    v1.operator+=({1, 2});
    assert(v1.x == 5 && v1.y == 5);
  }

  {
    Vector v1{3, 1};
    auto v2{++v1};
    assert(v2.x == 4 && v2.y == 2);
    assert(v1.x == 4 && v1.y == 2);
  }

  {
    Vector v1{3, 1};
    auto v2{v1++};
    assert(v2.x == 3 && v2.y == 1);
    assert(v1.x == 4 && v1.y == 2);
  }

  {
    Vector v1{3, 1};
    // auto x{&v1}; // Error: deleted
  }

  {
    Vector const v1{3, 1};
    assert(v1[0] == 3 && v1[1] == 1);

    Vector v2{3, 1};
    assert(v2[0] == 3 && v2[1] == 1);
    v2[0] = 10;
    assert(v2[0] == 10 && v2[1] == 1);
  }
}

// _____________________________________________________________________________
//  Operators as non members functions

auto operator+(Vector const &lhs, Vector const &rhs) -> Vector {
  auto vector{lhs};
  vector += rhs;
  return vector;
}

auto operator<<(std::ostream &os, Vector &src) -> std::ostream & {
  os << src.x << " " << src.y;
  return os;
}

auto operator>>(std::istream &is, Vector &src) -> std::istream & {
  is >> src.x >> src.y;
  return is;
}

auto non_member_operators() {
  {
    Vector v1{3, 1};
    Vector v2{3, 1};
    Vector v3{v1 + v2};
    assert(v3.x == 6 && v3.y == 2);
  }

  {
    Vector v3{6, 2};
    std::stringstream ss;
    ss << v3;
    assert(ss.str() == "6 2");

    Vector v4{0, 0};
    if (ss >> v4) {
      assert(v4.x == 6 && v4.y == 2);
    } else {
      assert(false);
    }
    assert(!ss.fail());
  }

  {
    std::stringstream ss;
    ss << "6 a";

    Vector v5{0, 0};
    if (ss >> v5) {
      assert(false);
    } else {
      assert(true);
    }
    assert(ss.fail());
  }
}

// _____________________________________________________________________________

auto run() -> void {
  function_object();
  litterals();
  member_operators();
  non_member_operators();
}

} // namespace operators
