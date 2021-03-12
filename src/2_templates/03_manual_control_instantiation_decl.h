#ifndef manual_control_instantiation_impl_h
#define manual_control_instantiation_impl_h

namespace manual_control_instantiation {

template <typename T> struct MCI_Class {
  auto foo() -> int { return 10; };
};

template <typename T> auto mci_func() -> int { return 10; };

} // namespace manual_control_instantiation

#endif
