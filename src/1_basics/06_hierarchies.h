#ifndef hierarchies_h
#define hierarchies_h

namespace hierarchies {

// ____________________________________________________________________________
// Abstract class

class AbstractClass {

public:
  // A virtual member function is sometimes called method
  virtual auto abstract_member() const noexcept
      -> void = 0; // Pure virtual method
  virtual ~AbstractClass(){};
};

// ____________________________________________________________________________
// Some classes

class ClassA {
protected:
  // visibility: members functions and friends of this class and
  //             members functions and friends of derived classes
  auto a_protected_func() const noexcept -> void{};

public:
  explicit ClassA(int value) {}
  virtual ~ClassA(){};

  auto a_public_func() const -> void{};
  auto abstract_member() const noexcept -> std::string {
    return "ClassA_abstract_member";
  }
};

class ClassB {
public:
  ClassB() {}
  explicit ClassB(bool value) {}
  virtual ~ClassB(){};

  auto abstract_member() const noexcept -> std::string {
    return "ClassB_abstract_member";
  }
};

// ____________________________________________________________________________
/*
A derived class inherits access to every member of a base class except:
its constructors and its destructor
its assignment operator members (operator=)
its friends
its private members

Access to base classes
- The derived is a subtype of its base:
  - public (default if the derived is a struct)
    - public members of the base class become public members of the derived
      class
    - protected members of the base class become protected members of the
      derived class
- The derived is not a subtype of its base:
  - private (default if the derived is a class)
    - public and protected members of the base class become private members of
      the derived class
  - protected
    - public and protected members of the base class become protected members of
      the derived class
*/
class Derived : public AbstractClass, protected ClassA, private ClassB {

public:
  // _____________________________________
  // Constructors
  Derived(bool value) : ClassA(10), ClassB(value) {}

  // Default constructor for the other classes
  Derived() : ClassA(10) {}

  // Override
  // Final (prevent futher overriding)
  //  void abstract_member() const noexcept override final
  auto abstract_member() const noexcept -> void override final {
    // A member of a derived class can use the public and protected members of
    // a base class
    a_public_func();
    a_protected_func();

    {
      // Explicit qualification ensures that the virtual mechanism is not used
      std::string x{ClassA::abstract_member()};

      Derived y{};
      y.abstract_member();
      y.ClassA::abstract_member();
    }
  }

  virtual ~Derived(){};
};

// Final class: prevent futher overriding
class FinalClass final : public Derived {
  auto baz() -> void {
    ClassA::abstract_member(); // OK: Derived: protected ClassA
    // ClassB::abstract_member(); // Error: Derived: private ClassB
  }
};

} // namespace hierarchies

#endif
