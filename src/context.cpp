#include "context.hpp"

#include <regex>

#include "Modules/Variables/var_manager.hpp"

namespace fel {

Context::Context() {}

Context::~Context() {}

std::string Context::parseVariableString(const std::string& unparsed) {
  if (module_variables_manager_.get() == nullptr)
    return unparsed;

  std::string str = unparsed;
  std::regex rgx("%([^%]*)%");
  std::sregex_iterator current(str.begin(), str.end(), rgx);
  std::sregex_iterator end;

  std::vector<std::string> matches;
  for (current; current != end; current++) {
    matches.push_back(current->str().substr(1, current->str().length() - 2));
  }

  for (size_t i = 0; i < matches.size(); ++i) {
    modules::variables::Var var = getModuleVariablesMemory()->get(matches[i], this, true);
    if (var.type != modules::variables::FelVarType::kNull)
      helper::str_replace_all(str, "%" + matches[i] + "%",
      (var.type == modules::variables::FelVarType::kString) ? var.string_value : std::to_string(var.float_value));
  }

  return str;
}

void Context::print(std::string str) {
  std::invoke(print_func_, str);
}

void Context::setPrintFunc(std::function<void(std::string str)> func) {
  print_func_ = func;
}

modules::variables::VariablesMemory * Context::getModuleVariablesMemory() {
  return module_variables_manager_->mem();
}

}
