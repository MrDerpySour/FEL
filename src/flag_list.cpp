#include "flag_list.hpp"

namespace fel {

FlagList::~FlagList() {
  destroy();
}

void FlagList::add(const int& id) {
  if (id < 0 || id > UINT16_MAX) {
    throw flag_invalid_id_exception();
  }

  if (!doesIdExist(id)) {
    try {
      data_.push_back(new Flag(id));
    } catch (flag_invalid_id_exception) {
      delete data_.back();
      data_.pop_back();
      throw flag_invalid_id_exception();
    }
  }
  else
    throw flag_redefinition_exception();
}

void FlagList::pop() {
  if (data_.size() > 0) {
    delete data_.back();
    data_.pop_back();
  }
}

void FlagList::destroy() {
  for (size_t i = 0; i < data_.size(); ++i) {
    if (data_[i] != nullptr)
      delete data_[i];
  }

  if (!data_.empty()) {
    data_.clear();
  }
}

Flag* FlagList::at_id(const int& id) {
  if (id < 0 || id > UINT16_MAX)
    return nullptr;

  for (size_t i = 0; i < data_.size(); ++i) {
    if (data_[i]->id() == id) {
      return data_[i];
    }
  }
  try {
    data_.push_back(new Flag(id));
    return data_.back();
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
