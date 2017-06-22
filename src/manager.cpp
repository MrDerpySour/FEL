#include "manager.hpp"

#include <fstream>
#include <regex>

#include "byte_code.hpp"
#include "flag_list.hpp"
#include "flag.hpp"

namespace fel {

Manager::Manager() {}

Manager::~Manager() {
  if (context_.flag_file_path_ != "") {
    saveFlags(context_.flag_file_path_);
  }
  
  if (context_.vars_file_path_ != "" && context_.getModuleVariablesMemory() != nullptr) {
    context_.getModuleVariablesMemory()->saveVariables(context_.vars_file_path_);
  }
}

void Manager::load(const std::string& file_path) {
  std::fstream file(file_path);
  if (file.fail())
    return;
    
  std::string scope = "";
  std::string line = "";
  bool debug_print_ = false;

  while (std::getline(file, line)) {
    if (line == "#debugprint") {
      debug_print_ = true;
    } else {
      Event evnt = Interpreter::inject(line, scope, &file, &context_);
      if (evnt.id != -1) {
        add(evnt, scope, false);
      }
    }
  }
  file.close();
  file_path_ = file_path;

  loadLinkedFiles();
  loadModules();
  
  if (context_.flag_file_path_ != "") {
    loadFlags(context_.flag_file_path_);
  }

  if (context_.vars_file_path_ != "" && context_.getModuleVariablesMemory() != nullptr) {
    context_.getModuleVariablesMemory()->loadVariables(context_.vars_file_path_, &context_);
  }

  if (debug_print_) {
    printDebug();
  }
}

bool Manager::execute(const int& event_id, const std::string& scope) {
  context_.scope = scope;

  bool result = executeSub(event_id);

  if (fatal_error_) {
    infinite_loop_.clear();

    while (!instruction_backups_.empty()) {
      instruction_backups_.pop();
    }

    while (!index_backups_.empty()) {
      index_backups_.pop();
    }

    context_.print(msg_);

    fatal_error_ = false;
    msg_ = "No error message available";
  }

  return result;
}

bool Manager::executeSub(const int& evnt_id) {

  // Catch infinite loop error
  if (std::find(infinite_loop_.begin(), infinite_loop_.end(), std::make_pair(context_.scope, evnt_id)) != infinite_loop_.end()) {

    fatal_error_ = true;
    msg_ = "Error: infinite loop detected\n";

    context_.current_instructions.clear();
    context_.instruction_index = 0;
    return false;
  }

  infinite_loop_.push_back(std::make_pair(context_.scope, evnt_id));

  // Load event and check for additional error checking
  auto group_it = events_.find(context_.scope.c_str());

  if (group_it == events_.end()) {
    context_.print("Error: no group found with name '" + context_.scope +"'\n");
    return false;
  }

  auto evnt_it = group_it->second.find(evnt_id);

  if (evnt_it == group_it->second.end()) {
    context_.print("Error: no event found with id '" + std::to_string(evnt_id) + "'\n");
    return false;
  }

  context_.current_instructions = evnt_it->second.instructions;

  // Error catching

  if (context_.current_instructions[0].byte_code == kFelError) {
    context_.print("Error: event '" + std::to_string(evnt_it->first) + "' didn't compile correctly\n");
    return false;
  }

  if (context_.current_instructions.size() < 2) {
    context_.print("Error: invalid amount of instructions parsed for event '" + std::to_string(evnt_it->first) + "'\n");
    return false;
  }

  if (context_.current_instructions[0].byte_code != kStart) {
    context_.print("Error: no starting point defined for event '" + std::to_string(evnt_it->first) + "'\n");
    return false;
  }

  if (context_.current_instructions[context_.current_instructions.size() - 1].byte_code != kEnd) {
    context_.print("Error: no end point defined for event '" + std::to_string(evnt_it->first) + "'\n");
    return false;
  }

  bool result = executeBytecode(evnt_it->second.id);

  if (!infinite_loop_.empty())
    infinite_loop_.pop_back();

  return result;
}

FEL_API bool Manager::add(const Event& evnt, const std::string& scope, const bool& update) {
  if (events_[scope].find(evnt.id) == events_[scope].end()) {
    events_[scope].insert(std::make_pair(evnt.id, evnt));
  } else {
    if (update) {
      events_[context_.scope][evnt.id] = evnt;
    } else {
      context_.print("Error: event with ID '" + std::to_string(evnt.id) +"' already exists\n");
      return false;
    }
  }

  return true;
}

bool Manager::executeBytecode(const int& event_executed) {

  for (context_.instruction_index = 0; context_.instruction_index < context_.current_instructions.size(); ++context_.instruction_index) {

    if (fatal_error_)
      return false;

    switch (context_.current_instructions[context_.instruction_index].byte_code) {

      case kFelError: {
        context_.print("Error: event '" + std::to_string(event_executed) + "' didn't compile correctly\n");
        return false; }

      case kStart: {
        break; }

      case kEnd: {
        return true; }

      case kFlj: {
        std::vector<std::string> tmp = helper::tokenize(context_.current_instructions[context_.instruction_index].parameters, '|');

        if (!(tmp.size() == 2 || tmp.size() == 3)) {
          context_.print("Error: invalid argument count (2/3 expected)\n");
          return false;
        }

        try {
          int flag_id = static_cast<int>(helper::parseMathString(context_.parseVariableString(tmp[0])));
          int evnt_id = static_cast<int>(helper::parseMathString(context_.parseVariableString(tmp[1])));

          if (flags_.at_id(flag_id) != nullptr) {
            if (flags_.at_id(flag_id)->is_set()) {
              context_.scope = (tmp.size() == 3) ? tmp[2] : "";

              return executeSub(evnt_id);
            }
          }
        } catch (...) {
          context_.print("Error: invalid argument type (int expected)\n");
          return false;
        }
        break; }

      case kFls: {
        try {
          int fls_flag_id = static_cast<int>(helper::parseMathString(context_.parseVariableString(context_.current_instructions[context_.instruction_index].parameters)));

          if (flags_.doesIdExist(fls_flag_id)) {
            flags_.at_id(fls_flag_id)->set();
          } else {
            try {
              flags_.add(fls_flag_id);
              if (flags_.at_id(fls_flag_id) != nullptr)
                flags_.at_id(fls_flag_id)->set();
            } catch (...) {
              context_.print("Error: invalid flag ID\n");
              return false;
            }
          }
        } catch (...) {
          context_.print("Error: invalid argument type (expected int)\n");
          return false;
        }
        break; }

      case kFlu: {
        try {
          int flu_flag_id = static_cast<int>(helper::parseMathString(context_.parseVariableString(context_.current_instructions[context_.instruction_index].parameters)));
          
          if (flags_.doesIdExist(flu_flag_id)) {
            flags_.at_id(flu_flag_id)->set(false);
          } else {
            try {
              flags_.add(flu_flag_id);
              if (flags_.at_id(flu_flag_id) != nullptr)
                flags_.at_id(flu_flag_id)->set(false);
            } catch (...) {
              context_.print("Error: invalid flag ID\n");
              return false;
            }
          }
        } catch (...) {
          context_.print("Error: invalid argument type (expected int)\n");
          return false;
        }
        break; }

      case kFlt: {
        try {
          int flt_flag_id = static_cast<int>(helper::parseMathString(context_.parseVariableString(context_.current_instructions[context_.instruction_index].parameters)));

          if (flt_flag_id < 0 || flt_flag_id > UINT16_MAX) {
            context_.print("Error: invalid flag ID\n");
            return false;
          }

          if (flags_.doesIdExist(flt_flag_id)) {
            if (flags_.at_id(flt_flag_id)->is_set())
              flags_.at_id(flt_flag_id)->set(false);
            else
              flags_.at_id(flt_flag_id)->set(true);
          } else {
            try {
              flags_.add(flt_flag_id);
              if (flags_.at_id(flt_flag_id) != nullptr)
                flags_.at_id(flt_flag_id)->set();
            } catch (...) {
              context_.print("Error: invalid flag ID\n");
              return false;
            }
          }
        } catch (...) {
          context_.print("Error: invalid argument type (expected int)\n");
          return false;
        }
        break; }

      case kPri: {
        context_.print(context_.parseVariableString(context_.current_instructions[context_.instruction_index].parameters));
        break; }

      case kEsc: {
        return true; }

      case kExe: {
        std::vector<std::string> tmp = helper::tokenize(context_.parseVariableString(context_.current_instructions[context_.instruction_index].parameters), '|');

        if (!(tmp.size() == 1 || tmp.size() == 2)) {
          context_.print("Error: invalid argument count (1/2 expected)\n");
          return false;
        }

        try {          
         int evnt_id = static_cast<int>(helper::parseMathString(context_.parseVariableString(context_.current_instructions[context_.instruction_index].parameters)));

         context_.scope = (tmp.size() == 2) ? tmp[1] : "";

         saveState();

         executeSub(evnt_id);

         restoreState();

        } catch (...) {
          context_.print("Error: invalid argument type (int expected)\n");
          return false;
        }
        break; }

      case kBld: {
        try {
          int bld_event_id = static_cast<int>(helper::parseMathString(context_.parseVariableString(context_.current_instructions[context_.instruction_index].parameters)));
          auto build_it = events_[context_.scope].find(bld_event_id);

          if (bld_event_id < 0) {
            context_.print("Error: invalid ID\n");
            return false;
          }

          Event evnt;
          Interpreter::compile(&evnt, file_path_, context_.scope, bld_event_id, &context_);

          add(evnt, context_.scope, true);
        } catch (...) {
          context_.print("Error: invalid argument type (int expected)\n");
          return false;
        }
        break; }

      case kSys: {
        system(context_.current_instructions[context_.instruction_index].parameters.c_str());
        break; }

      case kRpt: {
        std::vector<std::string> tmp = helper::tokenize(context_.current_instructions[context_.instruction_index].parameters, '|');

        if (!(tmp.size() == 2 || tmp.size() == 3)) {
          context_.print("Error: invalid argument count (2/3 expected)\n");
          return false;
        }

        int evnt_id = 0;
        int count = 0;

        try {
          evnt_id = static_cast<int>(helper::parseMathString(context_.parseVariableString(tmp[0])));
          count = static_cast<int>(helper::parseMathString(context_.parseVariableString(tmp[1])));
        } catch (...) {
          context_.print("Error: invalid argument type (int expected)\n");
          return false;
        }

        context_.scope = ((tmp.size() == 3) ? tmp[2] : "");

        saveState();

        for (int i = 0; i < count; ++i) {
          if (!fatal_error_)
            executeSub(evnt_id);
          else
            return false;
        }

        restoreState();
        break; }

      case kFelFunc: {
        if (context_.current_instructions[context_.instruction_index].parameters.length() <= 2) {
          context_.print("Error: invalid length\n");
          return false;
        }

        if (context_.current_instructions[context_.instruction_index].parameters[0] == '{') {
          std::string str = context_.current_instructions[context_.instruction_index].parameters.substr(1, context_.current_instructions[context_.instruction_index].parameters.length() - 1);
          size_t func_seperate_index = str.find_last_of('}');

          if (func_seperate_index == std::string::npos) {
            context_.print("Error: invalid function call\n");
            return false;
          }

          std::string tmp[2] = { str.substr(0, func_seperate_index), str.substr(func_seperate_index + 1) };

          try {
            auto it = custom_commands_.find(tmp[0]);

            if (it == custom_commands_.end())
              throw std::out_of_range("Key not found in map");
            
            it->second->execute(tmp[1], &flags_, &context_);
          } catch (...) {
            context_.print("Error: command '" + tmp[0] + "' not found\n");
            return false;
          }
        } else {
          context_.print("Error: invalid function call\n");
          return false;
        }

        break; }

      default: {
        context_.print("Error: undefined tokens found (token: '" + std::string(ByteCodeString[context_.current_instructions[context_.instruction_index].byte_code]) + "')\n");
        return false;
        break; }
    }
  }
  return false;
}

bool Manager::saveFlags(const std::string& file_path) {
  std::string content = "";

  for (size_t i = 0; i < flags_.size(); ++i) {
    content += std::to_string(flags_[i]->id()) + '=' + std::to_string(flags_[i]->is_set()) + '\n';
  }

  return helper::createFile(file_path, content);
}

bool Manager::loadFlags(const std::string& file_path) {
  std::fstream file(file_path);
  std::string line = "";

  if (!file.good())
    return false;

  std::vector<std::string> tmp;

  while (std::getline(file, line)) {
    if (line[0] != '#') {
      tmp = helper::tokenize(line, '=');
      if (tmp.size() != 2) {
        context_.print("Error: invalid markup\n");
        return false;
      }

      try {
        flags_.add(std::stoi(tmp[0]));
        (std::stoi(tmp[1]) == 1) ? flags_.at_id(std::stoi(tmp[0]))->set(true) : flags_.at_id(std::stoi(tmp[0]))->set(false);
      }
      catch (...) {
        context_.print("Error: invalid value type (int expected)\n");
        return false;
      }
    }
  }
  return true;
}

void Manager::registerFunction(const std::string& syntax, CommandClass* func) {
  
  if (std::string(syntax).find('-') != std::string::npos) {
    context_.print("Error: '-' is not allowed in function syntax\n");
    return;
  }

  std::string syntax_upper = syntax;

  if (context_.ignore_case_) {
    for (size_t i = 0; i < syntax_upper.size(); ++i) {
      syntax_upper[i] = static_cast<char>(toupper(syntax_upper[i]));
    }
  }

  if (!context_.ignore_case_) {
    for (int i = kFelBegin; i != kFelEnd; ++i) {
      if (strcmp(ByteCodeString[i], syntax.c_str()) == 0) {
        context_.print("Error: function overrides are not supported\n");
        return;
      }
    }
  } else {
    for (int i = kFelBegin; i != kFelEnd; ++i) {
      if (strcmp(ByteCodeString[i], syntax_upper.c_str()) == 0) {
        context_.print("Error: function overrides are not supported\n");
        return;
      }
    }
  }

  try {
    if (!context_.ignore_case_) {
      if (custom_commands_.find(syntax) == custom_commands_.end()) {
        custom_commands_.insert(std::make_pair(syntax, func));
      } else {
        context_.print("Error: command with that syntax '" + syntax + "' already exists\n");
      }
    } else {
      if (custom_commands_.find(syntax_upper) == custom_commands_.end()) {
        custom_commands_.insert(std::make_pair(syntax_upper, func));
      } else {
        context_.print("Error: command with that syntax '" + syntax_upper + "' already exists\n");
      }
    }
  } catch (...) {
    context_.print("Error: something went wrong registering function\n");
    return;
  }
}

const bool& Manager::is_set(const flag_id& id) {
  return flags_.at_id(id)->is_set();
}

void Manager::printDebug() {
  context_.print("Loaded modules:");
  for (size_t i = 0; i < context_.module_directives_.size(); ++i) {
    context_.print("\n\t-" + context_.module_directives_[i]);
  }

  context_.print("\n\n\nLinked files:");
  for (size_t i = 0; i < context_.link_directives_.size(); ++i) {
    context_.print("\n\t-" + context_.link_directives_[i]);
  }

  context_.print("\n\n\nEvents:");

  for (auto it = events_.begin(); it != events_.end(); ++it) {
    for (int i = 0; i < static_cast<int>(it->second.size()); ++i) {
      // Don't context_.print illegal events
      if (it->second[i].id != -1) {
        context_.print("\n " + (it->first == "") ? "" : ("'" + std::string(it->first) + "'") + " " + std::to_string(it->second[i].id) + ":\n");

        for (size_t instr = 0; instr < it->second[i].instructions.size(); ++instr) {
          context_.print("\t" + std::string(ByteCodeString[it->second[i].instructions.at(instr).byte_code]) + "\t[ " + it->second[i].instructions[instr].parameters + " ]\n");
        }
      }

      context_.print("\n");
    }
  }

  context_.print("\n---\n\n");
}

void Manager::loadLinkedFiles() {
  // A list of already loaded files to avoid double loading
  std::vector<std::string> link_directives;
  link_directives.push_back(file_path_);

  std::string scope = "";

  // For opening the files
  std::fstream file;
  std::string line;
  
  bool already_linked = false;
  for (size_t i = 0; i < context_.link_directives_.size(); ++i) {

    for (size_t j = 0; j < link_directives.size(); ++j) {
      if (context_.link_directives_[i] == link_directives[j]) {
        already_linked = true;
        break;
      }
    }

    if (!already_linked) {
      file.open(context_.link_directives_[i]);

      if (file.fail()) {
        context_.print("Error: could not open link directive\n\tPath: " + context_.link_directives_[i] +"\n");
        return;
      }

      while (std::getline(file, line)) {
        Event evnt = Interpreter::inject(line, scope, &file, &context_);
        if (evnt.id != -1) {
          add(evnt, scope, false);
        }
      }

      file.close();
      link_directives.push_back(context_.link_directives_[i]);
    }

    already_linked = false;
  }

  // With the double loaded files gone, we set the context's link directives to the correct one
  context_.link_directives_ = link_directives;
  link_directives.clear();
}

void Manager::loadModules() {
  // A list of already loaded modules to avoid double loading
  std::vector<std::string> module_directives;

  //
  bool already_loaded = false;
  for (size_t i = 0; i < context_.module_directives_.size(); ++i) {

    for (size_t j = 0; j < module_directives.size(); ++j) {
      if (context_.module_directives_[i] == module_directives[j]) {
        already_loaded = true;
        break;
      }
    }

    if (!already_loaded) {
      
      std::string module_name = context_.module_directives_[i];
      if (module_name == "variables") {
        context_.module_variables_manager_ = std::make_unique<modules::variables::VariablesManager>(this);
        context_.module_variables_manager_->reg();
      } else {
        context_.print("Error: module not found\n\tModule '" + module_name +"'\n");
        return;
      }

      module_directives.push_back(context_.module_directives_[i]);
    }

    already_loaded = false;
  }
}

void Manager::saveState() {
  // Save current state
  instruction_backups_.push(context_.current_instructions);
  index_backups_.push(context_.instruction_index);
}

void Manager::restoreState() {
  // Load current state
  if (!instruction_backups_.empty()) {
    context_.current_instructions = instruction_backups_.top();
    instruction_backups_.pop();
  }

  if (!index_backups_.empty()) {
    context_.instruction_index = index_backups_.top();
    index_backups_.pop();
  }
}

}  // namespace fel
