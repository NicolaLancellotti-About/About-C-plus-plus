#ifndef manual_control_instantiation_h
#define manual_control_instantiation_h

#include "03_manual_control_instantiation_decl.h"

namespace manual_control_instantiation {

// Not instantiate
extern template class MCI_Class<bool>;
extern template int MCI_Class<double>::foo();
extern template int mci_func<int>();

} // namespace manual_control_instantiation

#endif
