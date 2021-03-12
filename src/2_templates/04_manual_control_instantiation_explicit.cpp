#include "03_manual_control_instantiation_decl.h"

namespace manual_control_instantiation {

// Explicit instantiation
template class MCI_Class<bool>; // every member function is also instantiated
template int MCI_Class<double>::foo();
template int mci_func<int>();

} // namespace manual_control_instantiation
