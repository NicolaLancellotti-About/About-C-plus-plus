#include "../header.h"
#include <stdexcept>

namespace casts {

// _____________________________________________________________________________
// Narrow Cast

template <class Target, class Source> auto narrow_cast(Source v) -> Target {
  auto r{static_cast<Target>(v)};
  if (static_cast<Source>(r) != v) {
    throw std::runtime_error("narrow_cast<>() failed");
  }
  return r;
}

// _____________________________________________________________________________
// Dynamic cast

auto dynamic_cast_test() -> void {
  {
    class Base {};
    class Derived : public Base {};

    Base *b{new Derived{}};

    // Derived* d{dynamic_cast<Derived*>(b)}; // 'Base' is not polymorphic
    delete b;
  }

  {
    class Base {
    public:
      virtual ~Base() {}
      virtual auto foo() -> void{};
    };
    class Derived : public Base {};

    {
      Base *b{new Derived{}};
      Derived *d{dynamic_cast<Derived *>(b)};
      // d wil be nullptr if is not a Derived*
      assert(d != nullptr);
      delete b;
    }

    {
      Derived d{};
      Base &b_ref{d};
      try {
        Derived &d_ref{dynamic_cast<Derived &>(b_ref)};
        d_ref.foo();
      } catch (std::bad_cast) {
      }
    }
  }
}

// _____________________________________________________________________________

auto run() -> void {
  float const f{3.14};
  int i;

  // _____________________________________
  // static_cast
  // Convert between related types
  // No error but not necessarily safe.

  i = static_cast<int>(f);
  // Base* b{new Derived()};
  // Derived* d2{static_cast<Derived*>(b)};

  // _____________________________________
  // reinterpret_cast
  // Convert between unrelated types
  int p{0};
  int *p2{reinterpret_cast<int *>(p)};
  assert(p2 == nullptr);

  // _____________________________________
  // const_cast
  // Convert between types that differ only
  // in const and volatile qualifiers
  const char *cs{"text"};
  char *s{const_cast<char *>(cs)};
  assert(s == cs);

  // _____________________________________
  // dynamic_cast
  dynamic_cast_test();

  // _____________________________________
  // Function style cast
  i = int(f);

  // _____________________________________
  // C style cast
  i = (int)f;

  // _____________________________________
  // narrow_cast
  auto throwed{false};
  try {
    narrow_cast<char>(1000);
  } catch (...) {
    throwed = true;
  }
  assert(throwed);
}

} // namespace casts
