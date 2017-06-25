#ifndef FEL_CONTEXT_HPP_
#define FEL_CONTEXT_HPP_

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <functional>

#include "byte_code.hpp"

namespace fel {

// Forward declarations
class Manager; // manager.h

namespace modules { namespace variables { class VariablesManager; class VariablesMemory; } }

class Context {
friend class Manager;
friend class Interpreter;
 public:
  Context();
  ~Context();

  /**
   * Parses a string containing variables
   * @param unparsed The unparsed string
   * @return The parsed string
   */
  std::string parseVariableString(const std::string& unparsed);

  void print(std::string str);

  /**
   * @param method The method of printing
   */
  void setPrintFunc(std::function<void(std::string str)> func);

  // Getters

  /**
   * @return Whether #debug has been called or not
   */
  const bool& debugMode() const { return debug_mode_; }

  /**
   * @return Whether #ignorecase has been called or not
   */
  const bool& ignoreCase() const { return ignore_case_; }

  /**
   * @return All called #link directives
   */
  const std::vector<std::string>& getLinkDirectives() const { return link_directives_; }

  /**
   * @return All called #module directives
   */
  const std::vector<std::string>& getModuleDirectives() const { return module_directives_; }

  /**
   * @return The #flags filepath (if any)
   */
  const std::string& getFlagsFilePath() const { return flag_file_path_; }

  /**
   * @return The #vars filepath (if any)
   */
  const std::string& getVarsFilePath() const { return vars_file_path_; }

  /**
   * @return A pointer to the VariablesManager, returns nullptr if it doesn't exist
   */
  modules::variables::VariablesMemory* getModuleVariablesMemory();

  // Instructions
  std::vector<Instruction> current_instructions;
  size_t instruction_index = 0;

  // Scope
  std::string scope = "";

 private:
  // Modules
  std::unique_ptr<modules::variables::VariablesManager> module_variables_manager_;

  // Build info
  std::vector<std::string> link_directives_;
  std::vector<std::string> module_directives_;
  std::string flag_file_path_ = "";
  std::string vars_file_path_ = "";
  bool debug_mode_ = false;
  bool ignore_case_ = false;

  // Print function
  std::function<void(std::string str)> print_func_ = [&](std::string str) {
    std::cout << str;
  };
};

} // namespace fel

#endif // FEL_CONTEXT_HPP_
