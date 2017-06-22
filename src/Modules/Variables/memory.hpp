#ifndef FEL_VARIABLES_MEMORY_HPP_
#define FEL_VARIABLES_MEMORY_HPP_

#include "../../fel.hpp"
#include "../../context.hpp"

#include <unordered_map>

#include "variable.hpp"

namespace fel {
namespace modules {
namespace variables {

class VariablesMemory {
friend class FelRegisterVar;
friend class FelSetVar;
friend class FelDeleteVar;
 public:
  FEL_API bool add(const Var& var, Context* context);

  FEL_API bool remove(const std::string& name, Context* context, const bool& supress = false);

  FEL_API Var get(const std::string& name, Context* context, const bool& supress = false) const;

  FEL_API bool saveVariables(const std::string& path);

  FEL_API bool loadVariables(const std::string& path, Context* context);

 private:
  std::unordered_map<std::string, Var> variables_;
};

} // namespace variables
} // namespace modules
} // namespace fel

#endif // FEL_VARIABLES_MEMORY_HPP_
