#ifndef FEL_COMMAND_HPP_
#define FEL_COMMAND_HPP_

#include "fel.hpp"

#include "flag_list.hpp"
#include "context.hpp"

namespace fel {

class CommandClass {
 public:
   FEL_API virtual void execute(const std::string& parameters, FlagList* list, Context* context) = 0;
};

}  // namespace fel

#endif  // FEL_COMMAND_HPP_
