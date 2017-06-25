#ifndef FEL_VARIABLES_VARIABLE_HPP_
#define FEL_VARIABLES_VARIABLE_HPP_

#include <string>

namespace fel {
namespace modules {
namespace variables {

enum class FelVarType {
  kNull,
  kString,
  kFloat
};

struct Var {
  Var() {}

  Var(const std::string& name,
         const FelVarType& type,
         const std::string& val) :
    name(name),
    type(type),
    string_value(val) {}


  Var(const std::string& name,
         const FelVarType& type,
         const float& val) :
    name(name),
    type(type),
    float_value(val) {}

  std::string name = "null";
  FelVarType type = FelVarType::kNull;
  std::string string_value = "";
  float float_value = 0.0f;
};

} // namespac variables
} // namespace modules
} // namespace fel

#endif // FEL_VARIABLES_VARIABLE_HPP_
