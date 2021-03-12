#include "../header.h"
#include "04_classes.h"

namespace classes {

// _____________________________________
// Members

// Member functions
auto ClassWithMembers::member_function(){};
auto ClassWithMembers::member_function_static() -> void{};
// Data members
int ClassWithMembers::_data_member_static_1{0};

// ____________________________________________________________________________
// Move and copy class

void swap(MoveAndCopyClass &lhs, MoveAndCopyClass &rhs) noexcept {
  using std::swap;
  swap(lhs._value, rhs._value);
}

// _____________________________________________________________________________
// Friends

auto friend_function(ClassWithFriends const &x) -> int { return x._value; }

class FriendClass {
public:
  auto value(ClassWithFriends &x) -> int { return x._value; }
};

class A {};

// _____________________________________________________________________________

auto run() -> void {
  {
    // Const and logical constnes
    auto x{ConstAndLogicalConstnes{}};
    x.set_value(10);
    assert(x.value() == 10);

    auto const &ref_x{x};
    assert(ref_x.value() == 10);
    // ref_x.set_value(10); // Error
  }

  {
    // Delete Constructor
    struct Z {
      Z(double) {}     // can initialize with a double
      Z(int) = delete; // but not with an integer
    };
    Z z{1.1};
    // Z z{1}; // Error
  }

  {
    class NotOnFreeStore {
      void *operator new(size_t) = delete;
    };
    // auto x{std::make_unique<NotOnFreeStore>()}; // Error
  }

  {
    // Prevent destruction
    class NotOnStack1 {
      ~NotOnStack1() = delete;
    };
    // NotOnStack1 x1{}; // Error

    class NotOnStack2 {
    private:
      NotOnStack2() {}
    };
    // NotOnStack2 x2{}; // Error

    NotOnStack1 *x3{new NotOnStack1{}};
    // delete x3; // Error
  }
}

} // namespace classes
