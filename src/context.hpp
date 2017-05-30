#ifndef FEL_CONTEXT_HPP_
#define FEL_CONTEXT_HPP_

#include "fel.hpp"

#include <memory>

#include <vector>
#include <string>

#include "byte_code.hpp"

namespace fel {

// Forward declarations
class Manager; // manager.h

namespace modules { namespace variables { class VariablesManager; class VariablesMemory; } }

class Context {
friend class Manager;
friend class Interpreter;
 public:
  FEL_API Context();
  FEL_API ~Context();

  /**
   * Parses a string containing variables
   * @param unparsed The unparsed string
   * @return The parsed string
   */
  FEL_API std::string parseVariableString(const std::string& unparsed);

  // Getters

  /**
   * @return Whether #debug has been called or not
   */
  FEL_API const bool& debugMode() const { return debug_mode_; }

  /**
   * @return Whether #ignorecase has been called or not
   */
  FEL_API const bool& ignoreCase() const { return ignore_case_; }

  /**
   * @return All called #link directives
   */
  FEL_API const std::vector<std::string>& getLinkDirectives() const { return link_directives_; }

  /**
   * @return All called #module directives
   */
  FEL_API const std::vector<std::string>& getModuleDirectives() const { return module_directives_; }

  /**
   * @return The #flags filepath (if any)
   */
  FEL_API const std::string& getFlagsFilePath() const { return flag_file_path_; }

  /**
   * @return The #vars filepath (if any)
   */
  FEL_API const std::string& getVarsFilePath() const { return vars_file_path_; }

  /**
   * @return A pointer to the VariablesManager, returns nullptr if it doesn't exist
   */
  FEL_API modules::variables::VariablesMemory* getModuleVariablesMemory();

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
};

} // namespace fel

#endif // FEL_CONTEXT_HPP_