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
   * Interprets an event
   * @param evnt A pointer to the Event to compile
   * @param file_path The filepath to the event file
   * @param event_id What event to load, -1 = all
   */
  FEL_API static void compile(Event* evnt,
                      const std::string& file_path,
                      const int& event_id = -1);

  /**
   * Creates an event from a string of code
   * @param code The code to compile it from
   * @return An event created with the code
   */
  FEL_API static Event inject(const std::string& code, Context* context);

  /**
   * Compile an event with a specific string of code
   * @param evnt A pointer to the event to compile it to
   * @param code The string of code to compile
   */
  FEL_API static void inject(Event* evnt, const std::string& code, Context* context);
};

}  // namespace fel

#endif  // FEL_INTERPRETER_HPP_
