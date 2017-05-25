
# Flags
-----

Flags are a core component of the language.  
FEL being inspired by Cave Story's [TSC language](https://www.cavestory.org/guides/basicmodding/guide/tsc.html) and seeing the power of flags has made this a core feature.

## Creating & setting flags

Creating a flag is simple; it's automatically created once you do any flag-related commands.

### FLS
Using `FLS` on a flag will set it to `true`.  
If the flag doesn't exist, it will create it and set it to `true`.

### FLU
Using `FLU`on a flag will set it to `false`.
If the flag doesn't exist, it will create it and set it to `false`.

### FLT
Using `FLT` on a flag will set it to the opposite, if it's `true` it will set it to `false` and vice versa.
If the flag doesn't exist, it will create it and set it to `true` (flags are initialized as false).

### FLJ
`FLJ` will create the flag that's being passed in, it will initialize it as `false` so `FLJ` with a flag that doesn't yet exist will always fail.
