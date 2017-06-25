#include "var_commands.hpp"

#include "manager.hpp"

namespace fel {
namespace modules {
namespace variables {

void FelRegisterVar::execute(const std::string& parameters, FlagList*, Context* context) {
  std::vector<std::string> tmp = helper::tokenize(context->parseVariableString(parameters), '|');
  Var var;

  if (tmp.size() != 3) {
    context->print("Error: invalid argument count (3 expected)\n");
    return;
  }

  if (tmp[0].find('%') != std::string::npos) {
    context->print("Error: '%%' is not allowed in variable names\n");
    return;
  }

  if (tmp[0].length() == 0) {
    context->print("Error: variable name can't be empty\n");
    return;
  }

  if (tmp[0] == "id") {
    context->print("Error: 'id' is a preserved name\n");
    return;
  }

  if (tmp[0] == "scope") {
    context->print("Error: 'scope' is a preserved name\n");
    return;
  }

  var.name = tmp[0];

  if (tmp[1] == "float")
    var.type = FelVarType::kFloat;
  else if (tmp[1] == "string")
    var.type = FelVarType::kString;
  else {
    context->print("Error: unknown variable type '" + tmp[1] + "' detected\n");
    return;
  }

  if (var.type == FelVarType::kFloat) {
    try {
      var.float_value = std::stof(tmp[2]);
    } catch (...) {
      context->print("Error: invalid variable value '" + tmp[2] + "'\n");
    }
  } else
    var.string_value = tmp[2];

  memory_->add(var, context);
}

void FelSetVar::execute(const std::string& parameters, FlagList*, Context* context) {
  std::vector<std::string> tmp = helper::tokenize(context->parseVariableString(parameters), '|');
  if (memory_->variables_.find(tmp[0]) != memory_->variables_.end()) {
    Var var = memory_->get(tmp[0], context);
    switch (var.type) {
      case FelVarType::kString: {
        memory_->variables_[tmp[0]].string_value = tmp[1];
        break; }

      case FelVarType::kFloat: {
        memory_->variables_[tmp[0]].float_value = static_cast<float>(helper::parseMathString(tmp[1]));
        break; }
    }
  } else {
    context->print("Error: variable '" + tmp[0] + "' not found\n");
  }
}

void FelCompareVar::execute(const std::string& parameters, FlagList*, Context* context) {
  std::vector<std::string> tmp = helper::tokenize(context->parseVariableString(parameters), '|');

  if (!(tmp.size() == 3 || tmp.size() == 4)) {
    context->print("Error: invalid argument count (3/4 expected)");
  }

  if (tmp[0] == tmp[1]) {
    try {
      int evnt_id = static_cast<int>(helper::parseMathString(context->parseVariableString(tmp[2])));

      context->scope = (tmp.size() == 3) ? "" : tmp[3];

      parent_->executeSub(evnt_id);
    } catch (...) {
      context->print("Error: invalid argument type (int expected)\n");
      return;
    }
  }
}

} // namespace fel
} // namespace modules
} // namespace variables