<h3 align="center"><a href="index">Index</a></h3>

# Syntax
-----
NOTE: The `scope` parameter is always optional. If it isn't set, [global scope](scope) is assumed

## Core

| Syntax | Meaning     | Description                             | Parameters                     |
|:-------|:------------|:----------------------------------------|:-------------------------------|
| FLJ    | Flag jump   | Jumps to `event_id` if `flag_id` is set | `flag_id`, `event_id`, `scope` |
| FLS    | Flag set    | Sets flag `flag_id`                     |  `flag_id`                     |
| FLU    | Flag unset  | Unsets flag `flag_id`                   | `flag_id`                      |
| FLT    | Flag toggle | Toggles flag `flag_id`                  | `flag_id`                      |
| PRI    | Print       | Prints a message to the console         | `message`                      |
| ESC    | Escape      | Escapes from an event                   | None                           |
| EXE    | Execute     | Executes event `event_id`               | `event_id`, `scope`            |
| BLD    | Build       | Builds/rebuilds event `event_id`        | `event_id`                     |
| SYS    | System      | Executes a system command               | `command`                      |
| RPT    | Repeat      | Repeats an event                        | `event_id`, `count`, `scope`   |

## Modules
-----
### Variables

| Syntax | Meaning  | Description                                   | Parameters                          |
|:-------|:---------|:----------------------------------------------|:------------------------------------|
| REG    | Register | Registers a variable                          | `name`, `type`, `value`             |
| DEL    | Delete   | Deletes a variable                            | `name`                              |
| SET    | Set      | Sets a variable                               | `name`, `value`                     |
| CMP    | Compare  | Compares 2 values, jumps to event_id if equal | `val1`, `val2`, `event_id`, `scope` |

<h3 align="center"><a href="structure">Next</a></h3>
<h3 align="center"><a href="index">Index</a></h3>
