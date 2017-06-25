#ifndef FEL_VARIABLES_MANAGER_HPP_
#define FEL_VARIABLES_MANAGER_HPP_

#include "var_memory.hpp"
#include "var_commands.hpp"

namespace fel {

// Forward declarations
class Manager; // manager.h

namespace modules {
namespace variables {

class VariablesManager {
friend class Manager;
 public:
  VariablesManager(fel::Manager* parent) :
    regcmd_(&memory_),
    setcmd_(&memory_),
    delcmd_(&memory_),
    cmpcmd_(parent),
    parent_(parent) {}

  void reg();

  VariablesMemory* mem() { return &memory_; }

 private:
  VariablesMemory memory_;
  fel::Manager*   parent_ = nullptr;

  FelRegisterVar regcmd_;
  FelSetVar      setcmd_;
  FelDeleteVar   delcmd_;
  FelCompareVar  cmpcmd_;
};

} // namespace variables
} // namespace modules
} // namespace fel

#endif // FEL_VARIABLES_MANAGER_HPP_
