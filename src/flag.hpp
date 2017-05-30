#ifndef FEL_FLAG_HPP_
#define FEL_FLAG_HPP_

#include "fel.hpp"

#include <string>

typedef uint16_t flag_id;

namespace fel {

class flag_invalid_id_exception : public std::exception {
public:
  FEL_API virtual const char* what() const throw() {
    return "invalid flag ID\n";
  }
};

class Flag {
public:
  /**
   * @param id The id to assign to the flag
   * @throw flag_invalid_id_exception
   */
  FEL_API Flag(const int& id) {
    if (id < 0 || id > UINT16_MAX) {
      throw flag_invalid_id_exception();
    } else {
      id_ = static_cast<flag_id>(id);
    }
  };

  FEL_API ~Flag() {};

  /**
   * @return Whether or not the flag is set
   */
  FEL_API const bool& is_set() const { return state_; }

  /**
   * Sets the state of the flag
   * @param state The new state
   */
  FEL_API void set(const bool& state = true) { state_ = state; }

  /**
   * @return The flag's id
   */
  FEL_API const flag_id& id() const { return id_; }

  /* Operators */

  FEL_API explicit operator bool() { return state_ == true; }

  FEL_API bool operator==(const flag_id& id) { return id_ == id; }
  FEL_API bool operator==(const int& id) { return id_ == id; }

private:
  flag_id id_;
  bool state_ = false;
  // TEST
  
};

} // namespace fel

#endif  // FEL_FLAG_HPP_
