#ifndef Classes_h
#define Classes_h

namespace classes {

// ____________________________________________________________________________
// A Class is a namespace containing its members
// in a non-const member function of class X, `this` is an rvalue of type X*
// in a const member function of class X, `this` is an rvalue of type const X*

class ClassWithMembers {
public:
  // _____________________________________
  // Member functions
  auto member_function();
  inline auto member_function_inlined_1() -> void;
  auto member_function_inlined_2() -> void{};

  static auto member_function_static() -> void;

private:
  // Visibility: members functions and friends
  // _____________________________________
  // Data members
  int _data_member{};
  int const _data_member_const{};
  // non-static data member cannot be constexpr
  // int constexpr _data_member_constexpr{0};

  // Cannot be initialized directly in the class
  static int _data_member_static_1;
  // Can be initialized directly in the class
  inline static int _data_member_static_2{};

  static int constexpr _data_member_static_constexpr{};
  static int const _data_member_static_const{};
};

auto member_function_inlined_1() -> void{};

// ____________________________________________________________________________
// Move and copy class

class MoveAndCopyClass {
public:
  // Constructor
  explicit MoveAndCopyClass(int value) : _value{new int{value}} {}

  // Destructor
  ~MoveAndCopyClass() { delete _value; }

  // Copy constructor
  MoveAndCopyClass(MoveAndCopyClass const &src)
      : _value{src._value ? new int{*src._value} : nullptr} {}

  // Move constructor
  MoveAndCopyClass(MoveAndCopyClass &&src) noexcept : MoveAndCopyClass{} {
    swap(*this, src);
  }

  // Assignment
  MoveAndCopyClass &operator=(MoveAndCopyClass rhs) {
    swap(*this, rhs);
    return *this;
  }

private:
  int *_value{nullptr};

  MoveAndCopyClass() = default;
  friend void swap(MoveAndCopyClass &lhs, MoveAndCopyClass &rhs) noexcept;
};

// ____________________________________________________________________________
// Move only class

class MoveOnlyClass {
public:
  // Delete copy constructor & copy assignment
  MoveOnlyClass(MoveOnlyClass const &) = delete;
  MoveOnlyClass &operator=(MoveOnlyClass const &) = delete;

  // Constructor
  explicit MoveOnlyClass(int value) : _value{new int{value}} {}

  // Destructor
  ~MoveOnlyClass() { delete _value; }

  // Move constructor
  MoveOnlyClass(MoveOnlyClass &&src) noexcept : MoveOnlyClass() {
    *this = std::move(src);
  }

  // Move assignment
  MoveOnlyClass &operator=(MoveOnlyClass &&rhs) noexcept {
    if (this != &rhs) {
      _value = std::exchange(rhs._value, nullptr);
    }
    return *this;
  }

private:
  int *_value{nullptr};

  MoveOnlyClass() = default;
};

// ____________________________________________________________________________
// Const and logical constnes

class ConstAndLogicalConstnes {
private:
  int _value{};

  // Mutable mean that it can be modified even in const object
  mutable int _logical_constness{};

public:
  auto set_value(int value) -> void { _value = value; }

  // _____________________________________
  // Constant Member Functions
  // The member functions of a const object can only be called if they are
  // themselves specified as const members
  // Const does not apply to objects accessed through pointers or references
  auto value() const -> int {
    _logical_constness++;
    // _value++; // Error
    return _value;
  }
};

// _____________________________________________________________________________
// Class with friends
// The members declared friend have access to the implementation of the class

class A; // Forward Declaration

class ClassWithFriends {
public:
  friend auto friend_function(ClassWithFriends const &x) -> int;
  friend class FriendClass;
  friend A;

private:
  int _value{10};
};

// ____________________________________________________________________________
// Members of structs or unions have public access by default

struct AStruct {
  auto foo() -> void {}
};

union AUnion {
  int intValue;
  float floatValue;
  auto foo() -> void {}
};

} // namespace classes

#endif
