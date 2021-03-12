#include "NLPersonRef.h"
#include "Person.h"
#include <cstring>
#include <iostream>

using namespace call_cpp_from_c;

static inline NLPersonRef wrap(Person *person) {
  return reinterpret_cast<NLPersonRef>(person);
}

static inline Person *unwrap(NLPersonRef personRef) {
  return reinterpret_cast<Person *>(personRef);
}

// _____________________________________________________________________________

NLPersonRef NLPersonCreate(const char *name) { return wrap(new Person(name)); }

void NLPersonDelete(NLPersonRef personRef) { delete unwrap(personRef); }

char *NLPersonGetName(NLPersonRef personRef) {
  return strdup(unwrap(personRef)->name().c_str());
}

void NLPersonSetName(NLPersonRef personRef, const char *name) {
  unwrap(personRef)->setName(name);
}

void NLStringDelete(char *string) { free(string); }
