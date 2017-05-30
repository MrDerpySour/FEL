#ifndef FEL_BYTE_CODE_HPP_
#define FEL_BYTE_CODE_HPP_

#include "fel.hpp"

#include <string>

namespace fel {

/// This enum must always be in-sync with the ByteCodeString array in byte_code.cpp
enum ByteCode {
  kStart, // Start
  kFlj,
  kFls,
  kFlu,
  kFlt,
  kPri,
  kEsc,
  kExe,
  kBld,
  kSys,
  kRpt,
  kEnd,                // End
  kFelNone,            // No bytecode, used for default initialization
  kFelFunc,            // Custom command
  kFelError,           // Contains errors, shouldn't execute
  kFelBegin = kStart,  // Iterator first
  kFelEnd = kEnd       // Iterator last
};

extern const char* ByteCodeString[];

struct Instruction {
  FEL_API Instruction() {}

  /**
   * @param byte_code The bytecode of the instruction
   * @param parameters The parameters of the instruction
   */
  FEL_API Instruction(const ByteCode& byte_code, const std::string& parameters) :
    byte_code(byte_code), parameters(parameters) {}

  ByteCode byte_code;
  std::string parameters;
};

}  // namespace fel

#endif  // FEL_BYTE_CODE_HPP_
