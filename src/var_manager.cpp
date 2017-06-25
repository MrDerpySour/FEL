#include "var_manager.hpp"

#include "manager.hpp"

void fel::modules::variables::VariablesManager::reg() {
  try {
    parent_->registerFunction("REG", &regcmd_);
    parent_->registerFunction("SET", &setcmd_);
    parent_->registerFunction("DEL", &delcmd_);
    parent_->registerFunction("CMP", &cmpcmd_);
  } catch (...) {
    parent_->context()->print("Error: something went wrong registering function\n");
  }
}
