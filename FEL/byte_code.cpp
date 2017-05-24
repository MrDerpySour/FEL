#include "byte_code.hpp"

namespace fel {

const char* ByteCodeString[]{
  "START",  // ----------------------------------------------------------------------------- //
  "FLJ",    //  Flag jump       | Jumps to event_id if flag_id is set | [flag_id|event_id]   //
  "FLS",    //  Flag set        | Sets flag flag_id                   | [flag_id]            //
  "FLU",    //  Flag unset      | Unsets flag flag_id                 | [flag_id]            //
  "FLT",    //  Flag toggle     | Toggles flag flag_id                | [flag_id]            //
  "PRI",    //  Print           | Prints a message to the console     | [msg]                //
  "ESC",    //  Escape          | Escapes from an event               |                      //
  "EXE",    //  Execute         | Executes event event_id             | [event_id]           //
  "BLD",    //  Build           | Builds/rebuilds an event            | [event_id]           //
  "SYS",    //  System          | Executes a system command           | [command]            //
  "RPT",    //  Repeat          | Repeats an event                    | [event_id|count]     //
  "END",    // ----------------------------------------------------------------------------- //
  "NULL",
  "FUNC",
  "ERROR"
};

}  // namespace fel
