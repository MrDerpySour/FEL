#ifndef FEL_INTERPRETER_HPP_
#define FEL_INTERPRETER_HPP_

#include "fel.hpp"

#include "event.hpp"
#include "flag.hpp"
#include "flag_list.hpp"
#include "context.hpp"

namespace fel {

class Interpreter {
 public:
  /**
   * Compiles a specific event
   * @param evnt A pointer to the event to compile it into
   * @param file_path The filepath the event is in
   * @param scope The scope the event is in
   * @param evnt_id The ID of the event
   * @param context The current context
   */
  FEL_API static void compile(Event* evnt,
                              const std::string& file_path,
                              const std::string& scope,
                              const int& evnt_id,
                              Context* context);

  /**
   * Creates an event from a string of code
   * @param code The code to compile it from
   * @param scope The current scope
   * @param file A pointer to the current file
   * @param context A pointer to the context
   * @return An event created with the code
   */
  FEL_API static Event inject(const std::string& code,
                              std::string& scope,
                              std::fstream* file,
                              Context* context);

  /**
   * Compile an event with a specific string of code
   * @param evnt A pointer to the event to compile it into
   * @param code The code to compile it from
   * @param scope The current scope
   * @param file A pointer to the current file
   * @param context A pointer to the context
   */
  FEL_API static void inject(Event* evnt,
                             const std::string& code,
                             std::string& scope,
                             std::fstream* file,
                             Context* context);
};

}  // namespace fel

#endif  // FEL_INTERPRETER_HPP_
