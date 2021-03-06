#include "interpreter.hpp"

#include <fstream>
#include <string.h>

#include "helper.hpp"

static bool multi_line_comment = false;
static bool multi_line_event = false;

namespace fel {

void Interpreter::compile(Event* evnt,
                          const std::string& file_path,
                          const std::string& scope,
                          const int& evnt_id,
                          Context* context) {
  std::fstream file(file_path);
  std::string line = "";
  std::string current_scope = "";

  if (file.fail()) {
    context->print("Error: something went wrong loading file '" + file_path + "'\n");
    return;
  }

  while (std::getline(file, line)) {
    if (line.size() < 2)
      continue;

    if (line[0] == '/' && line[1] == '/')
      continue;

    // Parse group
    if (line[0] == '#') {
      size_t hash_seperator = line.find_first_of(' ');

      if (hash_seperator == std::string::npos) {
        hash_seperator = line.size();
      }

      std::string cmd = line.substr(1, hash_seperator - 1);

      if (cmd == "scope") {
        current_scope = line.substr(hash_seperator + 2, line.length() - hash_seperator - 3);
        continue;
      }

      if (cmd == "endscope") {
        current_scope = "";
        continue;
      }
    }

    // Parse event id
    size_t i = line.find_first_of('>');

    if (i != std::string::npos) {
      int id = std::stoi(line.substr(0, i));

      if (current_scope == scope && evnt_id == id) {
        std::string dummy = "";
        Interpreter::inject(evnt, line, dummy, &file, context);
        return;
      }
    } else {
      continue;
    }
  }

  context->print("Error: no event found with ID '" + std::to_string(evnt_id) + ((scope == "") ? "" : (" in scope " + scope).c_str()) + "\n");
}

Event Interpreter::inject(const std::string& code,
                          std::string& scope,
                          std::fstream* file,
                          Context* context) {
  Event evnt;
  inject(&evnt, code, scope, file, context);
  return evnt;
}

void Interpreter::inject(Event* evnt,
                         const std::string& code,
                         std::string& scope,
                         std::fstream* file,
                         Context* context) {
  std::string line = code;

  if (line.length() >= 2) {
    // If line is a comment
    if (line[0] == '/' && line[1] == '/') {
      evnt->id = -1;
      return;
    } else if (line[0] == '/' && line[1] == '*') {
      evnt->id = -1;
      multi_line_comment = true;
      return;
    } else if (line[line.length() - 2] == '*' && line.back() == '/') {
      evnt->id = -1;
      multi_line_comment = false;
      return;
    }

    if (multi_line_comment) {
      evnt->id = -1;
      return;
    }

    // Hash directives
    if (line[0] == '#' && context != nullptr) {
      size_t hash_seperator = line.find_first_of(' ');

      // No code parsing will be done
      evnt->id = -1;

      if (hash_seperator == std::string::npos) {
        hash_seperator = static_cast<int>(line.size());
      }

      std::string cmd = line.substr(1, hash_seperator - 1);

      // Set up link directives
      if (cmd == "link") {
        std::string path = line.substr(hash_seperator + 2, line.length() - hash_seperator - 3);

        if (helper::fileExists(path)) {
          context->link_directives_.push_back(path);
        } else {
          context->print("Error: link path '" + path + "' doesn't exist\n");
        }
        return;
      }
      
      // Set up module directives 
      if (cmd == "module") {
        context->module_directives_.push_back(line.substr(hash_seperator + 2, line.length() - hash_seperator - 3));
        return;
      }

      // Set up load directives
      if (cmd == "flags") {
        std::string path = line.substr(hash_seperator + 2, line.length() - hash_seperator - 3);
        context->flag_file_path_ = path;
        return;
      }

      if (cmd == "vars") {
        std::string path = line.substr(hash_seperator + 2, line.length() - hash_seperator - 3);
        context->vars_file_path_ = path;
        return;
      }

      // Enable ignorecase
      if (cmd == "ignorecase") {
        context->ignore_case_ = true;
        return;
      }

	    // Debugging
	    if (cmd == "debug") {
		    context->debug_mode_ = true;
		    return;
	    }

      // Groups
      if (cmd == "scope") {
        scope = line.substr(hash_seperator + 2, line.length() - hash_seperator - 3);
        context->scope = scope;
        return;
      }

      if (cmd == "endscope") {
        scope = "";
        context->scope = scope;
        return;
      }

      // If it didn't match any, give an error
      context->print("Error: link directive '" + cmd + "' is not recognized\n");
    }

    size_t i = line.find_first_of('>');

    try {
      if (i != std::string::npos && !multi_line_event) {
        evnt->id = std::stoi(line.substr(0, i));

        // If event is empty:
        // n>|
        // n>-|
        std::string righthand_code = line.substr(i + 1);
        helper::str_replace_all(righthand_code, "-", "");
        if (righthand_code == "|") {
          evnt->instructions = { Instruction(kStart, ""), Instruction(kEnd, "") };
          return;
        }
      } else if (i != std::string::npos && multi_line_event) {
        context->print("Error: event '" + std::to_string(evnt->id) + "' leaks into event '" + line.substr(0, i) + "'\n");
        evnt->instructions = { Instruction(kFelError, "") };
        return;
      }
    }
    catch (...) {
      context->print("Error: ID couldn't be parsed\n");
      evnt->instructions = { Instruction(kFelError, line) };
      return;
    }
  } else {
    return;
  }

  // Check if end is defined
  if (line.back() != '|' && line.back() != '$') {
    context->print("Error: no end marker found on event '" + std::to_string(evnt->id) + "'\n");
    evnt->instructions = { Instruction(kFelError, line) };
    return;
  } else if (line.back() == '|') {
    multi_line_event = false;
  }else if (line.back() == '$') {
    multi_line_event = true;
  }

  // Generate unique replacement token
  // Don't add the '<' yet
  std::string dash_replacement = std::to_string(helper::getInt(INT16_MAX, INT32_MAX)) + ":DASH_REPLACEMENT>";

  // Replace dashes with the unique replacement token
  bool in_params = false;
  for (size_t i = 0; i < line.length(); ++i) {
    if (line[i] == '[') {
      in_params = true;
    } else if (line[i] == ']') {
      in_params = false;
    }

    if (in_params && line[i] == '-') {
      line[i] = '<';
      line.insert(i + 1, dash_replacement);
    }
  }

  // Tokenize it
  std::vector<std::string> tmp_unparsed_cmds = helper::tokenize(line.substr(line.find_first_of('>') + 1), '-');
  std::vector<std::string> unparsed_cmds;

  // Revert back dashes in parameters
  for (std::string cmd : tmp_unparsed_cmds) {
    helper::str_replace_all(cmd, "<" + dash_replacement, "-");
    unparsed_cmds.push_back(cmd);
  }

  // No longer needed
  tmp_unparsed_cmds.clear();

  // Remove trailing '|' at the last element
  unparsed_cmds.back() = unparsed_cmds.back().substr(0, unparsed_cmds.back().length() - 1);

  std::vector<std::pair<std::string, std::string>> split_cmds;

  // Populate the split_cmds vector with the command and parameters
  for (size_t i = 0; i < unparsed_cmds.size(); ++i) {
    size_t index = unparsed_cmds[i].find_first_of('[');

    if (index == std::string::npos) {
      context->print("Error: no parameter block openeded for event '" + std::to_string(evnt->id) + "' on command '" + unparsed_cmds[i] +"'\n");
      evnt->instructions = { Instruction(kFelError, unparsed_cmds[i]) };
      return;
    }

    std::string tmp[2] = { unparsed_cmds[i].substr(0, index), unparsed_cmds[i].substr(index + 1) };

    // Error checking
    size_t closing_brace_index = tmp[1].find_last_of(']');
    
    if (closing_brace_index == std::string::npos) {
      context->print("Error: unmatched brackets in parameters block for event '" + std::to_string(evnt->id) + "' on command '" + tmp[0] + "'\n");
      evnt->instructions = { Instruction(kFelError, tmp[1]) };
      return;
    }
    
    if (tmp[1].substr(closing_brace_index).length() > 1) {
      context->print("Error: trailing tokens '" + tmp[1].substr(closing_brace_index + 1) + "' after parameters block found for event '" + std::to_string(evnt->id) + "' on command '" + tmp[0] + "'\n");
      evnt->instructions = { Instruction(kFelError, tmp[1].substr(closing_brace_index + 1)) };
      return;
    }
    

    split_cmds.push_back(std::make_pair(tmp[0], (tmp[1].substr(0, tmp[1].length() - 1))));
  }

  // Add the starting point
  evnt->instructions.push_back(Instruction(kStart, ""));

  for (size_t i = 0; i < split_cmds.size(); ++i) {
    Instruction instruction;

    // Replace %id% and %scope% with the actual variables
    // We replace the split_cmds instead of the parameters because it may cause problems with c_str or char buffers
    if (!split_cmds[i].second.empty()) {
      helper::str_replace_all(split_cmds[i].second, "%id%", std::to_string(evnt->id));
      helper::str_replace_all(split_cmds[i].second, "%scope%", context->scope);
      
      // Restore character codes
      helper::str_replace_all(split_cmds[i].second, "\\n", "\n");
      helper::str_replace_all(split_cmds[i].second, "\\t", "\t");
      helper::str_replace_all(split_cmds[i].second, "\\r", "\r");
      helper::str_replace_all(split_cmds[i].second, "\\b", "\b");
      helper::str_replace_all(split_cmds[i].second, "\\v", "\v");
      helper::str_replace_all(split_cmds[i].second, "\\f", "\f");
      helper::str_replace_all(split_cmds[i].second, "\\a", "\a");
      helper::str_replace_all(split_cmds[i].second, "\\0", "\0");
    }

    instruction.parameters = split_cmds[i].second;
    instruction.byte_code = kFelNone;

    std::string cmd = "";

    // Loop through the syntax and match it
    if (!context->ignore_case_) {
      cmd = split_cmds[i].first;

      for (int j = kFelBegin; j <= kFelEnd; ++j) {
        if (strcmp(split_cmds[i].first.c_str(), ByteCodeString[j]) == 0) {
          instruction.byte_code = ByteCode(j);
          break;
        }
      }
    } else {
      cmd = split_cmds[i].first;

      for (int j = kFelBegin; j <= kFelEnd; ++j) {
        for (size_t u = 0; u < cmd.size(); ++u) {
          cmd[u] = static_cast<char>(toupper(cmd[u]));
        }

        if (strcmp(cmd.c_str(), ByteCodeString[j]) == 0) {
          instruction.byte_code = ByteCode(j);
          break;
        }
      }
    }

    if (instruction.byte_code == kFelNone) {
      instruction.byte_code = kFelFunc;
      instruction.parameters = "{" + cmd + "}" + split_cmds[i].second;
    }

    evnt->instructions.push_back(instruction);
  }

  // Add the ending point
  if (!multi_line_event) {
    evnt->instructions.push_back(Instruction(kEnd, ""));
  } else {
    std::string new_code;
    std::getline(*file, new_code);
    inject(evnt, new_code, scope, file, context);
  }
}

}  // namespace fel
