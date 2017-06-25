#ifndef FEL_EVENT_HPP_
#define FEL_EVENT_HPP_

#include <vector>

#include "byte_code.hpp"

namespace fel {

struct Event {
  Event() {}

  /**
   * @param id The event's id
   * @param instructions The event's instructions
   */
  Event(const int& id, const std::vector<Instruction>& instructions) :
    id(id), instructions(instructions) {}

  int id = -1;
  std::vector<Instruction> instructions;
};

}  // namespace fel

#endif  // FEL_EVENT_HPP_
