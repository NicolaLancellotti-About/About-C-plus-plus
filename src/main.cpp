#include "header.h"

auto run() -> void {
  // basics
  values_types::run();
  functions::run();
  namespaces::run();
  classes::run();
  hierarchies::run();
  operators::run();
  exceptions::run();
  // templates
  templates::run();
  metaprogramming::run();
  // ffi
  ffi::run();
  // other
  casts::run();
  miscellaneous::run();
}

auto main(int argc, const char *argv[]) -> int try {
  run();
  return 0;
} catch (...) {
  return -1;
}
