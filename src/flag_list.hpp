#ifndef FEL_FLAG_LIST_HPP_
#define FEL_FLAG_LIST_HPP_

#include "fel.hpp"

#include <vector>
#include <memory>

#include "flag.hpp"

namespace fel {

/**
 * An exception that gets thrown when the ID was already present
 */
class flag_redefinition_exception : public std::exception {
 public:
  FEL_API virtual const char* what() const throw() {
    return "A flag with that ID already exists";
  }
};

class FlagList {
 public:
  FEL_API FlagList() {}
  FEL_API ~FlagList() {}

  /**
   * Adds a new Flag at the back
   * @param id The id of the new Flag
   * @throw flag_redefinition_exception
   */
  FEL_API void add(const int& id);

  /**
   * Pops the last Flag
   */
  FEL_API void pop();

  /**
   * Returns the flag at the given id, creates a new flag if it didn't exist already
   * @param id The id of the flag
   * @return A pointer to the flag at the given id
   */
  FEL_API Flag* at_id(const int& id);

  /**
   * @return How many elements are active in the list
   */
  FEL_API const size_t& size() const { return data_.size(); }

  /**
   * @param index The index of the Flag
   * @return The Flag at the given index, if the flag doesn't exist it will return nullptr
   */
  FEL_API Flag* at(const size_t& index) const {
    return (index < data_.size()) ? data_[index].get() : nullptr;
  }

  /**
   * @param index The index of the Flag
   * @return The Flag at the given index, if the flag doesn't exist it will return nullptr
   */
  FEL_API Flag* at(const int& index) const {
    if (index < 0)
      return nullptr;

    return at(static_cast<size_t>(index));
  }
  
  /* Operators */

  FEL_API Flag* operator[](const int& index) const {
    return at(index);
  }

  FEL_API Flag* operator[](const size_t& index) const {
    return at(index);
  }

  /**
   * A function to check whether an id is already present
   * @param id The id to check for
   * @return Whether or not the id is already present
   */
  FEL_API bool doesIdExist(const int& id);

private:
  std::vector<std::unique_ptr<Flag>> data_;
};

}  // namespace fel

#endif  // FEL_FLAG_LIST_HPP_
