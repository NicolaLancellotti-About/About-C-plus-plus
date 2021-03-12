#ifndef header_h
#define header_h

#include <cassert> // include c header in std namespace

// Preprocessor constant expression that evaluates to 1 if the file name is
// found and 0 if not
#if __has_include(<string>)
#include <string>
#endif

// const, constexpr, type aliases, inline, and anything declared static
// have internal linkage
int const N{0};
inline int inline_variable{10}; // Inline variable

// _____________________________________________________________________________
// Run

namespace values_types {
auto run() -> void;
}
namespace functions {
auto run() -> void;
}
namespace namespaces {
auto run() -> void;
}
namespace classes {
auto run() -> void;
}
namespace hierarchies {
auto run() -> void;
}
namespace operators {
auto run() -> void;
}
namespace exceptions {
auto run() -> void;
}
namespace templates {
auto run() -> void;
}
namespace metaprogramming {
auto run() -> void;
}
namespace casts {
auto run() -> void;
}
namespace miscellaneous {
auto run() -> void;
}
namespace ffi {
auto run() -> void;
}

#endif
