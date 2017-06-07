#include "var_commands.hpp"

#include "../../manager.hpp"

namespace fel {
namespace modules {
namespace variables {

void FelRegisterVar::execute(const std::string& parameters, FlagList*, Context* context) {
  std::vector<std::string> tmp = helper::tokenize(context->parseVariableString(parameters), '|');
  Var var;

  if (tmp.size() != 3) {
    printf("Error: invalid argument count (3 expected)\n");
    return;
  }

  if (tmp[0].find('%') != std::string::npos) {
    printf("Error: '%%' is not allowed in variable names\n");
    return;
  }

  if (tmp[0].length() == 0) {
    printf("Error: variable name can't be empty\n");
    return;
  }

  if (tmp[0] == "id") {
    printf("Error: 'id' is a preserved name\n");
    return;
  }

  if (tmp[0] == "scope") {
    printf("Error: 'scope' is a preserved name\n");
    return;
  }

  var.name = tmp[0];

  if (tmp[1] == "float")
    var.type = FelVarType::kFloat;
  else if (tmp[1] == "string")
    var.type = FelVarType::kString;
  else {
    printf("Error: unidentified variable type\n\tType '%s'\n", tmp[1].c_str());
    return;
  }

  if (var.type == FelVarType::kFloat) {
    try {
      var.float_value = std::stof(tmp[2]);
    } catch (...) {
      printf("Error: invalid variable value\n\tValue '%s'\n", tmp[2].c_str());
    }
  } else
    var.string_value = tmp[2];

  memory_->add(var);
}

void FelSetVar::execute(const std::string& parameters, FlagList*, Context* context) {
  std::vector<std::string> tmp = helper::tokenize(context->parseVariableString(parameters), '|');
  if (memory_->variables_.find(tmp[0]) != memory_->variables_.end()) {
    Var var = memory_->get(tmp[0]);
    switch (var.type) {
      case FelVarType::kString: {
        memory_->variables_[tmp[0]].string_value = tmp[1];
        break; }

      case FelVarType::kFloat: {
        memory_->variables_[tmp[0]].float_value = static_cast<float>(helper::parseMathString(tmp[1]));
        break; }
    }
  } else {
    printf("Error: variable not found\n\tVariable '%s'\n", tmp[0].c_str());
  }
}

void FelCompareVar::execute(const std::string& parameters, FlagList*, Context* context) {
  std::vector<std::string> tmp = helper::tokenize(context->parseVariableString(parameters), '|');

  if (!(tmp.size() == 3 || tmp.size() == 4)) {
    printf("Error: invalid argument count (3/4 expected)");
  }

  if (tmp[0] == tmp[1]) {
    try {
      int evnt_id = static_cast<int>(helper::parseMathString(context->parseVariableString(tmp[2])));

      context->scope = (tmp.size() == 3) ? "" : tmp[3];

      parent_->executeSub(evnt_id);
    } catch (...) {
      printf("Error: invalid argument type (int expected)\n");
      return;
    }
  }
}

} // namespace fel
} // namespace modules
} // namespace variables