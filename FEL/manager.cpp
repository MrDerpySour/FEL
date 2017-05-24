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
    
  std::string line = "";
  while (std::getline(file, line)) {
    if (line == "#debug") {
      context_.debug_mode_ = true;
    } else {
      Event evnt = Interpreter::inject(line, &context_);
      if (evnt.id != -1) {
        if (events_.find(evnt.id) == events_.end())
          events_.insert(std::make_pair(evnt.id, evnt));
        else {
          printf("Error: event with id already exists\n\tID: %d\n", evnt.id);
          return;
        }
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
    context_.getModuleVariablesMemory()->loadVariables(context_.vars_file_path_);
  }

  if (context_.debug_mode_) {
    printDebug();
  }
}

bool Manager::execute(const int& event_id) {
  bool result = executeEvent(event_id);

  if (fatal_error_) {
    fatal_error_ = false;
    msg_ = "No error message available";
  }

  return result;
}

bool Manager::executeEvent(const int& evnt_id) {
  auto it = events_.find(evnt_id);

  if (it == events_.end()) {
    printf("Error: no event found with id '%d'\n", evnt_id);
    return false;
  }

  context_.current_instructions = it->second.instructions;

  // Error catching

  if (context_.current_instructions[0].byte_code == kFelForbidden) {
    printf("Error: event '%d' didn't compile correctly\n", it->second.id);
    return false;
  }

  if (context_.current_instructions.size() < 2) {
    printf("Error: invalid amount of instructions parsed\n");
    return false;
  }

  if (context_.current_instructions[0].byte_code != kStart) {
    printf("Error: no starting point defined\n");
    return false;
  }

  if (context_.current_instructions[context_.current_instructions.size() - 1].byte_code != kEnd) {
    printf("Error: no end point defined\n");
    return false;
  }

  return executeBytecode(it->second.id);
}

bool Manager::executeBytecode(const int& event_executed) {

  if (checkFatalError() == true)
    return false;

  for (context_.instruction_index = 0; context_.instruction_index < context_.current_instructions.size(); ++context_.instruction_index) {

    if (fatal_error_)
      return false;

    switch (context_.current_instructions[context_.instruction_index].byte_code) {

      case kFelForbidden: {
        printf("Error: event '%d' didn't compile correctly\n", event_executed);
        return false; }

      case kStart: {
        break; }

      case kEnd: {
        return true; }

      case kFlj: {
        std::vector<std::string> tmp = helper::tokenize(context_.current_instructions[context_.instruction_index].parameters, '|');

        try {
          int flag_id = static_cast<int>(helper::parseMathString(context_.parseVariableString(tmp[0])));
          int evnt_id = static_cast<int>(helper::parseMathString(context_.parseVariableString(tmp[1])));

          if (flags_.at_id(flag_id) != nullptr) {
            if (flags_.at_id(flag_id)->is_set()) {
              checkInfiniteLoop(evnt_id);
              return this->executeEvent(evnt_id);
            }
          }
        } catch (...) {
          printf("Error: invalid argument type (int expected)\n");
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
              printf("Error: invalid flag ID\n");
              return false;
            }
          }
        } catch (...) {
          printf("Error: invalid argument type (expected int)\n");
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
              printf("Error: invalid flag ID\n");
              return false;
            }
          }
        } catch (...) {
          printf("Error: invalid argument type (expected int)\n");
          return false;
        }
        break; }

      case kFlt: {
        try {
          int flt_flag_id = static_cast<int>(helper::parseMathString(context_.parseVariableString(context_.current_instructions[context_.instruction_index].parameters)));

          if (flt_flag_id < 0 || flt_flag_id > UINT16_MAX) {
            printf("Error: invalid flag ID\n");
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
              printf("Error: invalid flag ID\n");
              return false;
            }
          }
        } catch (...) {
          printf("Error: invalid argument type (expected int)\n");
          return false;
        }
        break; }

      case kPri: {
        printf("%s", context_.parseVariableString(context_.current_instructions[context_.instruction_index].parameters).c_str());
        break; }

      case kEsc: {
        return true; }

      case kExe: {
        try {
          int evnt_id = static_cast<int>(helper::parseMathString(context_.parseVariableString(context_.current_instructions[context_.instruction_index].parameters)));

          checkInfiniteLoop(evnt_id);

          saveState();

          executeEvent(evnt_id);

          restoreState();

        } catch (...) {
          printf("Error: invalid argument type (int expected)\n");
          return false;
        }
        break; }

      case kBld: {
        try {
          int bld_event_id = static_cast<int>(helper::parseMathString(context_.parseVariableString(context_.current_instructions[context_.instruction_index].parameters)));
          auto build_it = events_.find(bld_event_id);

          if (bld_event_id < 0) {
            printf("Error: invalid ID\n");
            return false;
          }

          Event evnt;
          Interpreter::compile(&evnt, file_path_, bld_event_id);

          if (build_it == events_.end()) {
            events_.insert(std::make_pair(bld_event_id, evnt));
          } else {
            events_[bld_event_id] = evnt;
          }
        } catch (...) {
          printf("Error: invalid argument type (int expected)\n");
          return false;
        }
        break; }

      case kSys: {
        system(context_.current_instructions[context_.instruction_index].parameters.c_str());
        break; }

      case kRpt: {
        std::vector<std::string> tmp = helper::tokenize(context_.current_instructions[context_.instruction_index].parameters, '|');
        if (tmp.size() != 2) {
          printf("Error: invalid argument count (2 expected)\n");
          return false;
        }

        int evnt_id = 0;
        int count = 0;
        try {
          evnt_id = static_cast<int>(helper::parseMathString(context_.parseVariableString(tmp[0])));
          count = static_cast<int>(helper::parseMathString(context_.parseVariableString(tmp[1])));
        } catch (...) {
          printf("Error: invalid argument type (int expected)\n");
          return false;
        }

        saveState();

        checkInfiniteLoop(evnt_id);

        for (int i = 0; i < count; ++i) {
          if (!checkFatalError())
            executeEvent(evnt_id);
          else
            return false;
        }

        restoreState();

        break; }

      case kFelFunc: {
        if (context_.current_instructions[context_.instruction_index].parameters.length() <= 2) {
          printf("Error: invalid length\n");
          return false;
        }

        if (context_.current_instructions[context_.instruction_index].parameters[0] == '{') {
          std::string str = context_.current_instructions[context_.instruction_index].parameters.substr(1, context_.current_instructions[context_.instruction_index].parameters.length() - 1);
          int func_seperate_index = str.find_last_of('}');

          if (func_seperate_index == -1) {
            printf("Error: invalid function call\n");
            return false;
          }

          std::string tmp[2] = { str.substr(0, func_seperate_index), str.substr(func_seperate_index + 1) };

          try {
            auto it = custom_commands_.find(tmp[0]);

            if (it == custom_commands_.end())
              throw std::out_of_range("Key not found in map");
            
            it->second->execute(tmp[1], &flags_, &context_);
          } catch (...) {
            printf("Error: command '%s' not found\n", tmp[0].c_str());
            return false;
          }
        } else {
          printf("Error: invalid function call\n");
          return false;
        }

        break; }

      default: {
        printf("Error: undefined tokens found (token: '%s')\n", ByteCodeString[context_.current_instructions[context_.instruction_index].byte_code]);
        return false;
        break; }
    }
  }
}

bool Manager::saveFlags(const std::string& file_path) {
  std::string content = "";

  for (size_t i = 0; i < flags_.size(); ++i) {
    content += std::to_string(flags_[i].id()) + '=' + std::to_string(flags_[i].is_set()) + '\n';
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
        printf("Error: invalid markup\n");
        return false;
      }

      try {
        flags_.add(std::stoi(tmp[0]));
        (std::stoi(tmp[1]) == 1) ? flags_.at_id(std::stoi(tmp[0]))->set(true) : flags_.at_id(std::stoi(tmp[0]))->set(false);
      }
      catch (...) {
        printf("Error: invalid value type (int expected)\n");
        return false;
      }
    }
  }
  return true;
}

void Manager::registerFunction(const char* syntax, CommandClass* func) {

  std::string syntax_upper = syntax;

  if (context_.ignore_case_) {
    for (size_t i = 0; i < syntax_upper.size(); ++i) {
      syntax_upper[i] = toupper(syntax_upper[i]);
    }
  }

  if (!context_.ignore_case_) {
    for (int i = kFelBegin; i != kFelEnd; ++i) {
      if (strcmp(ByteCodeString[i], syntax) == 0) {
        printf("Error: function overrides are not supported\n");
        return;
      }
    }
  } else {
    for (int i = kFelBegin; i != kFelEnd; ++i) {
      if (strcmp(ByteCodeString[i], syntax_upper.c_str()) == 0) {
        printf("Error: function overrides are not supported\n");
        return;
      }
    }
  }

  try {
    if (!context_.ignore_case_) {
      if (custom_commands_.find(syntax) == custom_commands_.end()) {
        custom_commands_.insert(std::make_pair(syntax, func));
      } else {
        printf("Error: command with that syntax already exists\n\tSyntax '%s'\n", syntax);
      }
    } else {
      if (custom_commands_.find(syntax_upper) == custom_commands_.end()) {
        custom_commands_.insert(std::make_pair(syntax_upper, func));
      } else {
        printf("Error: command with that syntax already exists\n\tSyntax '%s'\n", syntax_upper.c_str());
      }
    }
  } catch (...) {
    printf("Error: something went wrong registering function\n");
    return;
  }
}

const bool& Manager::is_set(const flag_id& id) {
  return flags_.at_id(id)->is_set();
}

void Manager::printDebug() {
  printf("Loaded modules:");
  for (size_t i = 0; i < context_.module_directives_.size(); ++i) {
    printf("\n\t-%s", context_.module_directives_[i].c_str());
  }

  printf("\n\n\nLinked files:");
  for (size_t i = 0; i < context_.link_directives_.size(); ++i) {
    printf("\n\t-%s", context_.link_directives_[i].c_str());
  }

  printf("\n\n\nEvents:");

  for (size_t i = 0; i < events_.size(); ++i) {

    // For some reason when I iterate through the events, an event with id -1 shows up
    // .size() return 1 and trying to execute it gives an unidentified id error.
    // Stepping through this I noticed the for loop executing twice but when i was set to 1 it wouldn't
    if (events_[i].id != -1) {
      printf("\n  %d:\n", events_[i].id);

      for (size_t instr = 0; instr < events_[i].instructions.size(); ++instr) {
        printf("\t%s\t[ %s ]\n", ByteCodeString[events_[i].instructions.at(instr).byte_code], 
                                events_[i].instructions[instr].parameters.c_str());
      }
    }

    printf("\n");
  }
    
  printf("\n---\n\n");
}

bool Manager::checkFatalError() {
  if (fatal_error_) {
    infinite_loop_.clear();

    while (!instruction_backups_.empty()) {
      instruction_backups_.pop();
    }

    while (!index_backups_.empty()) {
      index_backups_.pop();
    }

    printf("%s", msg_.c_str());
  }

  return fatal_error_;
}

void Manager::loadLinkedFiles() {
  // A list of already loaded files to avoid double loading
  std::vector<std::string> link_directives;
  link_directives.push_back(file_path_);

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
        printf("Error: could not open link directive\n\tPath: %s\n", context_.link_directives_[i].c_str());
        return;
      }

      while (std::getline(file, line)) {
        Event evnt = Interpreter::inject(line, &context_);
        if (evnt.id != -1) {
          if (events_.find(evnt.id) == events_.end())
            events_.insert(std::make_pair(evnt.id, evnt));
          else {
            printf("Error: event with id already exists\n\tID: %d\n", evnt.id);
            return;
          }
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
        printf("Error: module not found\n\tModule '%s'\n", module_name.c_str());
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

bool Manager::checkInfiniteLoop(const int& evnt_id, const bool& add_id) {
  // Catch infinite loop error
  if (std::find(infinite_loop_.begin(), infinite_loop_.end(), evnt_id) != infinite_loop_.end()) {

    fatal_error_ = true;
    msg_ = "Error: infinite loop detected\n";

    context_.current_instructions.clear();
    context_.instruction_index = 0;
  }

  if (add_id)
    infinite_loop_.push_back(evnt_id);

  return fatal_error_;
}

}  // namespace fel
