#ifndef FEL_EVENT_HPP_
#define FEL_EVENT_HPP_

#include "fel.hpp"

#include <vector>

#include "byte_code.hpp"

namespace fel {

struct Event {
  FEL_API Event() {}
  FEL_API Event(const int& id, const std::vector<Instruction>& instructions) :
    id(id), instructions(instructions) {}

  int id = -1;
  std::vector<Instruction> instructions;
};

}  // namespace fel

#endif  // FEL_EVENT_HPP_
