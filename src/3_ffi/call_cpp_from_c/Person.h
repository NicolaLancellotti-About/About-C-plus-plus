#ifndef Person_hpp
#define Person_hpp

#include <string>

namespace call_cpp_from_c {

class Person {
  std::string _name;

public:
  Person(std::string name) : _name{name} {};

  std::string name() { return _name; }

  void setName(std::string name) { _name = name; }
};

} // namespace call_cpp_from_c

#endif
