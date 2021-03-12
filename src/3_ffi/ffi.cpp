#include "../header.h"
#include "call_c_from_cpp/cpplib.h"
#include "call_cpp_from_c/NLPersonRef.h"
#include <cstring>

// _____________________________________________________________________________
// Call C form C++

auto call_c_from_cpp() {
  assert(clib::sum(1, 2) == 3);
  assert(std::strcmp("ABC", "abc") == -1);
}

// _____________________________________________________________________________
// Call C++ form C

auto call_cpp_from_c() {
  NLPersonRef person{NLPersonCreate("Name1")};
  {
    char *name{NLPersonGetName(person)};
    assert(!strcmp(name, "Name1"));
    NLStringDelete(name);
  }

  NLPersonSetName(person, "Name2");

  {
    char *name{NLPersonGetName(person)};
    assert(!strcmp(name, "Name2"));
    NLStringDelete(name);
  }

  NLPersonDelete(person);
}

// _____________________________________________________________________________

extern "C" void ffi_func_1(); // declaration
extern "C" int ffi_x1;        // declaration

extern "C" {
void ffi_func_2(); // declaration
extern int ffi_x2; // declaration
int ffi_x3{};      // definition
};

auto ffi_func_1() -> void {} // definition
auto ffi_func_2() -> void {} // definition
int ffi_x1{0};               // definition
int ffi_x2{0};               // definition

// _____________________________________________________________________________

namespace ffi {

auto run() -> void {
  call_c_from_cpp();
  call_cpp_from_c();
}

} // namespace ffi
