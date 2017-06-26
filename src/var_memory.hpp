#ifndef FEL_VARIABLES_MEMORY_HPP_
#define FEL_VARIABLES_MEMORY_HPP_

#include "context.hpp"

#include <unordered_map>

#include "var_variable.hpp"

namespace fel {
namespace modules {
namespace variables {

class VariablesMemory {
friend class FelRegisterVar;
friend class FelSetVar;
friend class FelDeleteVar;
 public:
  bool add(const Var& var, Context* context);

  void remove(const std::string& name, Context* context);

  Var get(const std::string& name, Context* context, const bool& supress = false) const;

  bool saveVariables(const std::string& path);

  bool loadVariables(const std::string& path, Context* context);

 private:
  std::unordered_map<std::string, Var> variables_;
};

} // namespace variables
} // namespace modules
} // namespace fel

#endif // FEL_VARIABLES_MEMORY_HPP_
