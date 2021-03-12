#include "../header.h"
#include "06_hierarchies.h"

namespace hierarchies {

// ____________________________________________________________________________
// Slicing

auto slicing() -> void {
  {
    class Base {
    public:
      virtual auto f() -> std::string { return "base"; };
    };

    class Derived : public Base {
    public:
      virtual auto f() -> std::string { return "derived"; };
    };

    Derived derived{};
    Base base{derived};
    assert(base.f() == "base");
  }

  // Prevent
  {
    class Base {
    public:
      Base() = default;
      Base(Base const &) = delete; // - prohibit copying of the base class
      virtual auto f() -> std::string { return "base"; };
    };

    class Derived : public Base {
    public:
      virtual auto f() -> std::string { return "derived"; };
    };

    Derived derived{};
    // Base base{derived}; // Error
    assert(derived.f() == "derived");
  }

  {
    class Base {
    public:
      virtual auto f() -> std::string { return "base"; };
    };

    // make the base class a private or protected base
    class Derived : protected Base {
    public:
      virtual auto f() -> std::string { return "derived"; };
    };

    Derived derived{};
    // Base base{derived}; // Error
    assert(derived.f() == "derived");
  }
}

// ____________________________________________________________________________
// Function declared in different non-namespace scopes do not overload

namespace overloading {

class A {};
class B : public A {};

struct Base {
  Base() = default;
  Base(std::string value){};
  auto f(int x) -> std::string { return "base"; }
  auto g(B const &x) -> std::string { return "base"; }
  auto h(B const x) -> std::string { return "base"; }
};

struct Derived : Base {
  auto f(double x) -> std::string { return "derived"; }
  auto g(A const &x) -> std::string { return "derived"; }
  auto h(A const x) -> std::string { return "derived"; }
};

struct Derived2 : Base {
  using Base::Base; // inherit constructors
  using Base::f;
  using Base::g;
  using Base::h;
  auto f(double x) -> std::string { return "derived"; }
  auto g(A const &x) -> std::string { return "derived"; }
  auto h(A const x) -> std::string { return "derived"; }
};

auto overloading() {
  overloading::Derived d{};
  assert(d.f(10.0) == "derived");
  assert(d.g(A{}) == "derived");
  assert(d.h(A{}) == "derived");

  assert(d.f(10) == "derived");  // cast 10 to double
  assert(d.g(B{}) == "derived"); // A is not polymorphic
  assert(d.h(B{}) == "derived"); // slicing

  overloading::Derived2 d2{"124"}; // Base constructor
  assert(d2.f(10.0) == "derived");
  assert(d2.g(A{}) == "derived");
  assert(d2.h(A{}) == "derived");

  assert(d2.f(10) == "base");
  assert(d2.g(B{}) == "base");
  assert(d2.h(B{}) == "base");
}

} // namespace overloading

// ____________________________________________________________________________
// Virtual inheritance

auto virtual_inheritance() -> void {
  /*
   Every virtual base of a derived class is represented by the same shared
   object. A virtual base is always considered a direct base of its most derived
   class
   */

  class Base {
  public:
    Base(std::string v) : value{v} {};
    std::string value;
    auto foo() -> std::string { return value; };
  };

  // Virtual inheritance

  class A_virtualBase : virtual public Base {
  public:
    A_virtualBase() : Base("A_virtualBase"){};
  };
  class B_virtualBase : virtual public Base {
  public:
    B_virtualBase() : Base("B_virtualBase"){};
  };

  // Ordinary inheritance

  class A : public Base {
  public:
    A() : Base("A"){};
  };
  class B : public Base {
  public:
    B() : Base("B"){};
  };

  // Derived

  class Derived : public A_virtualBase,
                  public B_virtualBase,
                  public A,
                  public B {
  public:
    Derived() : Base("derived"){};
  };

  Derived c{};
  // auto x{c.value}; // Error

  assert(c.A_virtualBase::value == "derived");
  assert(c.B_virtualBase::value == "derived");
  assert(c.A::value == "A");
  assert(c.B::value == "B");

  c.A_virtualBase::value = "virtualA";
  c.B_virtualBase::value = "virtualB";
  c.A::value = "A";
  c.B::value = "B";

  assert(c.A_virtualBase::value == "virtualB");
  assert(c.B_virtualBase::value == "virtualB");
  assert(c.A::value == "A");
  assert(c.B::value == "B");

  // auto x{c.foo()}; // Error
  assert(c.A_virtualBase::foo() == "virtualB");
  assert(c.B_virtualBase::foo() == "virtualB");
  assert(c.A::foo() == "A");
  assert(c.B::foo() == "B");
}

// ____________________________________________________________________________

auto run() -> void {
  slicing();
  overloading::overloading();
  virtual_inheritance();
}

} // namespace hierarchies
