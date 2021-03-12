#include "../header.h"
#include <cstring>
#include <memory>

namespace values_types {

decltype(auto) f_decltype(int &x) { return x; }

auto f_auto(int &x) { return x; }

// _____________________________________________________________________________

auto variables_definitions() {

  {
    int v1 = 0; // C-like initialization
    int v2(0);  // Functional style initialization
    int v3{0};  // Uniform initialization
    assert(v1 == v2 && v2 == v3 && v3 == 0);

    v1 = 1; // Assignment
  }

  {
    // Type deduction
    int x{0};
    int &ref_x{x};
    int const const_x{0};
    int array_x[10];
    int const const_array_x[10]{};

    {
      auto v1{x}; // int

      // References are ignored
      auto v2{ref_x}; // int

      // const-volatile cv-qualifiers are ignored
      auto v3{const_x}; // int

      // array are degenerated into pointer types
      auto v4{array_x}; // int*

      // qualified auto deduced type
      auto const v5{array_x}; // int* const
      // v5 = x; // error
    }
    {
      auto &v6{const_x}; // int const&
      // v6 = 1; // error
      auto &v7{array_x};       // int (&)[10]
      auto &v8{const_array_x}; // int const (&)[10]
      // v8[1] = 0; // error
    }
    {
      auto v3 = {1}; // std::initializer_list (immutable)
      assert(*v3.begin() == 1);
    }
  }

  {
    // Decltype
    int array_x[10];
    decltype(array_x) v; // int [10]
  }

  {
    int x{0};
    int &x_ref{f_decltype(x)};
    int x2{f_auto(x)};
    x2++;
    assert(x == 0);
    x_ref++;
    assert(x == 1);
  }
}
// _____________________________________________________________________________
// Values

auto values() {
  {
    // Binary numbers
    int x{0b11};
    assert(x == 3);
  }

  {
    // Digit separators
    auto one_milion{1'000'000};
    assert(one_milion == 1000000);
  }

  {
    // Aggregate member initialization
    struct S {
      int a;
      const char *b;
      int c;
      char d{b[a]};
    };

    S s{1, "abc"};
    assert(s.a == 1);
    assert(std::string{s.b} == "abc");
    assert(s.c == 0);
    assert(s.d == 'b');
  }

  {
    // Structured binding declaration
    {
      int x[2]{1, 2};
      auto const &[a, b]{x};
      assert(a == 1 && b == 2);
    }
    {
      struct A {
        int a;
        int b;
      };
      A x{1, 2};
      auto const &[a, b]{x};
      assert(a == 1 && b == 2);
    }
    {
      std::pair x{2, 'a'};
      auto const &[a, b]{x};
      assert(a == 2 && b == 'a');
    }
  }
}

// _____________________________________________________________________________

auto enums() -> void {
  {
    enum class Day {
      sunday = 7,
      monday = 1,
      tuesday,
      wednesday,
      thursday,
      friday,
      saturday
    };
    auto day{Day::sunday};
    assert(day == Day::sunday);

    // No implicitly convertible to int
    assert(static_cast<int>(Day{}) == 0);
    assert(static_cast<int>(Day::sunday) == 7);
    assert(static_cast<int>(Day::monday) == 1);
    assert(static_cast<int>(Day::tuesday) == 2);
    assert(static_cast<Day>(6) == Day::saturday);
  }

  {
    // Declare underlying type
    enum class Planet : char {
      mercury,
      venus,
      earth,
      mars,
      jupiter,
      saturn,
      uranus,
      neptune
    };
    auto planet{Planet::mercury};
    assert(planet == Planet::mercury);
  }
}

// _____________________________________________________________________________

auto strings() -> void {
  std::string s0{"123"
                 "abc"};
  assert(s0 == "123abc");

  // In raw strings, backslashes and double quotes are valid characters
  const char *s1{R"(\n")"};
  const char *s2{"\\n\""};
  const char *s3{R"***(\n")***"};
  assert(std::strcmp(s1, s2) == 0);
  assert(std::strcmp(s1, s3) == 0);

  auto s4{R"(abc
  def)"};
  assert(std::strcmp(s4, "abc\n  def") == 0);
}

// _____________________________________________________________________________

auto pointers_references() -> void {

  struct AStruct {
    int int_value;
    int bool_value;

    AStruct(int int_value, int bool_value)
        : int_value{int_value}, bool_value{bool_value} {}
  };

  {
    // Pointers
    int *p{nullptr};

    // if allocation fail:
    p = new int{0}; // an exception is thrown
    delete p;
    p = new (std::nothrow) int{5}; // p will be equal to nullptr
    delete p;

    // Pointer to array
    int *pp{new int[2]{}};
    assert(pp[0] == 0 && p[1] == 0);
    delete[] pp;
    pp = new int[2]{0, 1};
    assert(pp[0] == 0 && p[1] == 1);
    delete[] pp;
  }

  {
    // References
    int v{0};
    int &ref{v};
    v = 10;
    assert(ref == 10);

    int const &ref_to_tmp{10}; // Temporary variable is created
    assert(ref_to_tmp == 10);
  }

  {
    // unique_ptr
    std::unique_ptr<AStruct> p;
    p = std::unique_ptr<AStruct>(new AStruct{10, true});
    p = std::make_unique<AStruct>(AStruct{10, true});
    p = std::make_unique<AStruct>(10, true);

    assert(p != nullptr);
    assert(p->int_value == 10);
    assert((*p).bool_value == true);
  }

  {
    // shared_ptr
    std::shared_ptr<AStruct> p;
    assert(p == nullptr);
    p = std::make_shared<AStruct>(11, false);
    assert(p != nullptr);
    assert(p->int_value == 11);
    assert((*p).bool_value == false);

    {
      // weak_ptr
      std::weak_ptr<AStruct> weak{p};
      assert(!weak.expired());
      std::shared_ptr<AStruct> p2{weak.lock()};
      assert(p2);

      p = p2 = nullptr;
      assert(weak.expired());
      p2 = weak.lock();
      assert(!p2);
    }
  }

  {
    // Pointers to members
    struct Base {
      int a;

      auto f(int b) -> void {}
      static auto g(int b) -> void {}
    };

    struct Derived : public Base {
      auto h(int b) -> void {}
    };

    {
      void (Base::*pf)(int){&Base::f};
      pf = &Base::f;
      void (*pg)(int){&Base::g};
      int Base::*pa{&Base::a};

      Base x{};
      (pg)(20);
      x.*pa = 10;
      (x.*pf)(20);

      using PF = void (Base::*)(int);
      PF pf2 = &Base::f;
      (x.*pf2)(20);
    }

    {
      // Contravariance
      void (Base::*base_p)(int){&Base::f};
      void (Derived::*derived_p)(int){&Derived::f};

      base_p = &Base::f;
      derived_p = &Base::f;

      // base_p = &Derived::h; // Error
      derived_p = &Derived::h;
    }
  }
}

// _____________________________________________________________________________

auto run() -> void {
  assert(N == 0);
  variables_definitions();
  values();
  enums();
  strings();
  pointers_references();
}

} // namespace values_types
