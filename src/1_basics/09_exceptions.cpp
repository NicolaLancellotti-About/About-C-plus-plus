#include "../header.h"

namespace exceptions {

// _____________________________________________________________________________
// Custom Exception

class CustomException final : public std::exception {
public:
  CustomException(std::string message) : message{message} {}

  auto what() const noexcept -> const char * override {
    return message.c_str();
  }

private:
  std::string message;
};

// _____________________________________________________________________________
// Nonthrowing functions

// The noexcept() operator takes an expression as argument and return true if
// the compiler knows that it cannot throw and false otherwise
// It is not completely checked by the compiler
// It terminate unconditionally if an exception is raised

auto nonthrowing_function_1() noexcept -> void {}
auto nonthrowing_function_2() noexcept(true) -> void {}

// _____________________________________________________________________________
// Throwing functions

auto throwing_function() noexcept(false) -> void {
  throw CustomException{"my error"};
}

// _____________________________________________________________________________
// Function try-block

auto function_try_block() -> void try {

} catch (...) {
}

// _____________________________________________________________________________

auto run() -> void {
  auto throwed{false};
  try {
    throwing_function();
#if 1
  } catch (CustomException &e) {
    assert(std::string{e.what()} == "my error");
    throwed = true;
  } catch (std::exception &e) {
    assert(false);
#else
  } catch (std::exception &e) {
    assert(std::string{e.what()} == "my error");
    throwed = true;
#endif
  } catch (int param) {
    assert(false);
    throw; // rethrow
  } catch (...) {
    assert(false);
  }
  assert(throwed);
}

} // namespace exceptions
