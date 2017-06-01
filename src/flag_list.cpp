#include "flag_list.hpp"

namespace fel {

void FlagList::add(const int& id) {
  if (id < 0 || id > UINT16_MAX) {
    throw flag_invalid_id_exception();
  }

  if (!doesIdExist(id)) {
    try {
      data_.push_back(std::make_unique<Flag>(id));
    } catch (flag_invalid_id_exception) {
      data_.back().reset();
      data_.pop_back();
      throw flag_invalid_id_exception();
    }
  }
  else
    throw flag_redefinition_exception();
}

void FlagList::pop() {
  if (data_.size() > 0) {
    data_.back().reset();
    data_.pop_back();
  }
}

Flag* FlagList::at_id(const int& id) {
  if (id < 0 || id > UINT16_MAX)
    return nullptr;

  for (size_t i = 0; i < data_.size(); ++i) {
    if (data_[i]->id() == id) {
      return data_[i].get();
    }
  }
  try {
    data_.push_back(std::make_unique<Flag>(id));
    return data_.back().get();
  } catch (flag_invalid_id_exception) {
    return nullptr;
  }
}

bool FlagList::doesIdExist(const int& id) {
  if (id < 0 || id > UINT16_MAX)
    return false;

  for (size_t i = 0; i < data_.size(); ++i) {
    if (data_[i]->id() == id)
      return true;
  }
  return false;
}

}  // namespace fel
