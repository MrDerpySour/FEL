#ifndef FEL_MANAGER_HPP_
#define FEL_MANAGER_HPP_

#include "fel.hpp"

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
#include "Modules/Variables/var_manager.hpp"

namespace fel {

class Manager {
 public:
  FEL_API Manager();
  FEL_API ~Manager();

  /**
   * Loads a file with multiple events with each event on a different line
   * @param file_path The filepath to the file
   */
  FEL_API void load(const std::string& file_path);
 
  /**
   * Executes an event
   * @param evnt_id The ID of the event
   * @return Whether the operation executed without any errors or not
   */
  FEL_API bool execute(const int& evnt_id, const std::string& group_name);

  /**
   * Execute a subsidairy event, meant to be used by custom commands
   * @param evnt_id The ID of the event
   * @return Whether the operation executed without any errors or not
   */
  FEL_API bool executeEvent(const int& evnt_id);

  /**
   * Saves all the flags
   * @param file_path Where to save the flags to
   * @return Whether the operation executed without any errors or not
   */
  FEL_API bool saveFlags(const std::string& file_path);

  /**
   * Loads the flags
   * @param file_path Filepath to the file
   * @return Whether the operation executed without any errors or not
   */
  FEL_API bool loadFlags(const std::string& file_path);

  /**
   * Registers a custom function
   * @param syntax The syntax for the function
   * @param func A pointer to the function
   */
  FEL_API void registerFunction(const char* syntax, CommandClass* func);

  /**
   * Checks whether or not a flag at id is set
   * @param id The flag_id to check
   */
  FEL_API const bool& is_set(const flag_id& id);

  /**
   * Prints out all debug information
   */
  FEL_API void printDebug();

  /**
   * @return A pointer to the manager's context
   */
  FEL_API const Context* context() const { return &context_; }

  /**
   * Checks if you're inside an infinite loop
   * @param group_name The name of the group the event is in
   * @param evnt_id The ID of the event you're going to call
   * @param add_id Whether or not to add the ID to the infinite loop's watch list
   * @return Whether or not you're executing an infinite loop or not
   */
  FEL_API bool checkInfiniteLoop(const std::string& group_name, const int& evnt_id, const bool& add_id = true);

  /**
   * Removes the id from the infite loop checker
   * @param evnt_id The ID to remove
   */
  FEL_API void noLoop();

  /**
   * Saves the current instructions
   */
  FEL_API void saveState();

  /**
   * Restores the last saved instructions
   */
  FEL_API void restoreState();

  /**
   * @return Whether or not a fatal error has occured
   * @return Whether there are any fatal errors or not
   */
  FEL_API bool checkFatalError();

 private:
  /**
   * Executes the bytecode
   * @return Whether the operation executed without any errors or not
   */
  FEL_API bool executeBytecode(const int& event_executed);
  
  /**
   * Loads all the linked files
   */
  FEL_API void loadLinkedFiles();

  /**
   * Loads all the modules
   */
  FEL_API void loadModules();
    
 private:
  // General
  std::unordered_map<std::string, std::unordered_map<int, Event>> events_;
  std::unordered_map<std::string, CommandClass*> custom_commands_;

  std::string file_path_ = "";

  FlagList flags_;
  Context context_;

  // For saving/loading states when executing multiple events
  std::stack<std::vector<Instruction>> instruction_backups_;
  std::stack<int> index_backups_;

  // Error detection
  std::vector<std::pair<std::string, int>> infinite_loop_;
  std::string msg_ = "No error message available";
  bool fatal_error_ = false;
};

}  // namespace fel

#endif  // FEL_MANAGER_HPP_
