#ifndef FEL_VARIABLES_COMMANDS_HPP_
#define FEL_VARIABLES_COMMANDS_HPP_

#include "../../fel.hpp"
#include "../../helper.hpp"

#include "memory.hpp"
#include "../../command.hpp"

namespace fel {
namespace modules {
namespace variables {

class FelRegisterVar : public CommandClass {
 public:
  FelRegisterVar(VariablesMemory* memory) :
    memory_(memory) {}

  FEL_API void execute(const std::string& parameters, FlagList* list, Context* context);

 private:
  VariablesMemory* memory_ = nullptr;
};

class FelSetVar : public CommandClass {
 public:
  FelSetVar(VariablesMemory* memory) :
    memory_(memory) {}

  FEL_API void execute(const std::string& parameters, FlagList* list, Context* context);

 private:
  VariablesMemory* memory_ = nullptr;
};
 
class FelDeleteVar : public CommandClass {
 public:
  FelDeleteVar(VariablesMemory* memory) :
    memory_(memory) {}

  FEL_API void execute(const std::string& parameters, FlagList*, Context* context) {
    memory_->remove(context->parseVariableString(parameters), context);
  }

 private:
  VariablesMemory* memory_ = nullptr;
};

class FelCompareVar : public CommandClass {
 public:
  FelCompareVar(fel::Manager* parent) :
    parent_(parent) {}

  FEL_API void execute(const std::string& parameters, FlagList* list, Context* context);

 private:
  fel::Manager* parent_ = nullptr;
};

} // namespace variables
} // namespace modules
} // namespace fel

#endif // FEL_VARIABLES_COMMANDS_HPP_
