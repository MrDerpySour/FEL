#include "memory.hpp"

#include <fstream>

#include "../../helper.hpp"

namespace fel {
namespace modules {
namespace variables {

bool VariablesMemory::add(const Var& var, Context* context) {
  if (var.type == FelVarType::kNull)
    return false;

  if (variables_.find(var.name) == variables_.end()) {
    variables_.insert(std::make_pair(var.name, var));
    return true;
  } else {
    context->print("Error: variable '" + var.name + "' already defined\n");
    return false;
  }
}

bool VariablesMemory::remove(const std::string& name, Context* context, const bool& supress) {
  if (variables_.find(name) != variables_.end()) {
    variables_.erase(name);
    return true;
  } else {
    if (!supress)
      context->print("Error: variable '" + name + "' does not exist\n");
    return false;
  }
}

Var VariablesMemory::get(const std::string& name, Context* context, const bool& supress)  const {
  if (variables_.find(name) != variables_.end()) {
    return variables_.at(name);
  } else {
    if (!supress)
      context->print("Error: variable '" + name + "' does not exist\n");
    return Var();
  }
}

bool VariablesMemory::saveVariables(const std::string& path) {
  std::string contents = "";

  for (auto it = variables_.begin(); it != variables_.end(); ++it) {
    contents += ">"
             +  it->second.name
             +  "|"
             +  ((it->second.type == FelVarType::kString) ? "string" : "float")
             +  "|"
             +  ((it->second.type == FelVarType::kString) ? it->second.string_value : std::to_string(it->second.float_value).c_str())
             +  '<'
             +  '\n';
  }

  return helper::createFile(path, contents);
}

bool VariablesMemory::loadVariables(const std::string& path, Context* context) {
  std::fstream file(path);

  if (file.fail())
    return false;

  std::string line;
  while (std::getline(file, line)) {
    if (line.front() == '>' && line.back() == '<') {
      std::vector<std::string> tmp = helper::tokenize(line.substr(1, line.length() - 2), '|');

      if (tmp.size() != 3)
        return false;

      FelVarType type;
      if (tmp[1] == "string")
        type = FelVarType::kString;
      else if (tmp[1] == "float")
        type = FelVarType::kFloat;
      else
        type = FelVarType::kNull;

      Var var;

      switch (type) {
        case FelVarType::kString: {
          var.type = type;
          var.name = tmp[0];
          var.string_value = tmp[2];
          break; }

        case FelVarType::kFloat: {
          var.type = type;
          var.name = tmp[0];
          var.float_value = std::stof(tmp[2]);
          break; }
      }

      add(var, context);
    }
  }

  return true;
}

} // namespace variables
} // namespace modules
} // namespace fel
