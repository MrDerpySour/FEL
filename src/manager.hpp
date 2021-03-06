#ifndef FEL_MANAGER_HPP_
#define FEL_MANAGER_HPP_

#include <memory>
#include <unordered_map>
#include <stack>

// Core
#include "event.hpp"
#include "flag.hpp"
#include "flag_list.hpp"
#include "interpreter.hpp"
#include "command.hpp"
#include "context.hpp"

// Modules
#include "var_manager.hpp"

namespace fel {

class Manager {
 public:
  Manager();

  /**
   * Loads a source file
   * @param file_path The filepath to the file
   */
  Manager(const std::string& file_path) {
    load(file_path);
  }

  /**
   * Sets a new print function
   * @param print_func The function used for all output
   */
  Manager(std::function<void(std::string)> print_func) {
    context_.setPrintFunc(print_func);
  }

  /**
   * Loads a source file and sets a new print function
   * @param file_path The filepath to the file
   * @param print_func The function used for all output
   */
  Manager(const std::string& file_path,
                  std::function<void(std::string)> print_func) {
    context_.setPrintFunc(print_func);
    load(file_path);
  }

  ~Manager();

  /**
   * Loads a source file
   * @param file_path The filepath to the file
   */
  void load(const std::string& file_path);
 
  /**
   * Executes an event
   * @param evnt_id The ID of the event
   * @return Whether the operation executed without any errors or not
   */
  bool execute(const int& evnt_id, const std::string& scope = "");

  /**
   * Execute a subsidairy event
   * Uses the context's scope.
   * @param evnt_id The ID of the event
   * @return Whether the operation executed without any errors or not
   */
  bool executeSub(const int& evnt_id);

  /**
   * Adds an event
   * @param evnt The event to add
   * @param scope The scope to put it in
   * @param update If the event already exists, update it
   * @return Whether or not the operation was successful
   */
  bool add(const Event& evnt, const std::string& scope = "", const bool& update = true);

  /**
   * Saves all the flags
   * @param file_path Where to save the flags to
   * @return Whether the operation executed without any errors or not
   */
  bool saveFlags(const std::string& file_path);

  /**
   * Loads the flags
   * @param file_path Filepath to the file
   * @return Whether the operation executed without any errors or not
   */
  bool loadFlags(const std::string& file_path);

  /**
   * Registers a custom function
   * @param syntax The syntax for the function
   * @param func A pointer to the function
   */
  void registerFunction(const std::string& syntax, CommandClass* func);

  /**
   * Checks whether or not a flag at id is set
   * @param id The flag_id to check
   */
  const bool& is_set(const flag_id& id);

  /**
   * Prints out all debug information
   */
  void printDebug();

  /**
   * @return A pointer to the manager's context
   */
  Context* context() { return &context_; }

  /**
   * Saves the current instructions
   */
  void saveState();

  /**
   * Restores the last saved instructions
   */
  void restoreState();

 private:
  /**
   * Executes the bytecode
   * @return Whether the operation executed without any errors or not
   */
  bool executeBytecode(const int& event_executed);
  
  /**
   * Loads all the linked files
   */
  void loadLinkedFiles();

  /**
   * Loads all the modules
   */
  void loadModules();
    
 private:
  // General
  std::unordered_map<std::string, std::unordered_map<int, Event>> events_;
  std::unordered_map<std::string, CommandClass*> custom_commands_;

  std::string file_path_ = "";

  FlagList flags_;
  Context context_;

  // For saving/loading states when executing multiple events
  std::stack<std::vector<Instruction>> instruction_backups_;
  std::stack<size_t> index_backups_;

  // Error detection
  std::vector<std::pair<std::string, int>> infinite_loop_;
  std::string msg_ = "No error message available";
  bool fatal_error_ = false;
};

}  // namespace fel

#endif  // FEL_MANAGER_HPP_
