<h3 align="center"><a href="index">Index</a></h3>

# Directives
-----

FEL has a few "preprocessor" directives.  
You can put directives anywhere in the file, as long is it's on its own line but keeping them at the top is recommended.  

A directive statement is made by typing a `#` followed by the command.  

Here is a table of all directives.

| Directive    | Description                      | Parameters |
|:-------------|:---------------------------------|:-----------|
| `link`       | Links another file               | "filepath" |
| `module`     | Loads a module                   | "name"     |
| `debug`      | Enables the context's debug bool | None       |
| `debugprint` | Prints out all the debug info    | None       |
| `flags`      | Sets flags filepath              | "filepath" |
| `vars`       | Sets vars filepath               | "filepath" |
| `ignorecase` | Disables case-sensitivity        | None       |
| `scope`      | Defines a scope                  | "name"     |
| `endscope`   | Returns to the global scope      | None       |

### Link

`link` will mark another file to be loaded by the interpeter.  
This is similar to `include` in C/C++.  

Example:
```
#link "another.fel"
```

### Module

`module` will load a [module](modules).  

Example:
```
#module "variables"
```

### Debug

`debug` will enable the `debug_mode_` bool in the Manager's [Context](context).

Example:
```
#debug
```

### Debugprint

`debugprint` will print out all the bytecode and build info after loading.

Example:
```
#debugprint
```

### Flags

This will set the save/load filepath for the [flags](flags).  
Flags will be saved/loaded in a format similar to .ini files.

Example:
```
#flags "flags.ini"
```

### Vars

This will set the save/load filepath for the [variables](variables).  
This will only take effect if the `variables` module is loaded.  
Vars will be saved/loaded in [fvt](fel-misc) format.

Example:
```
#vars "vars.fvt"
```

### Ingore case

Someone complaint that it was annoying to keep holding shift.  
`ignorecase` will remove the case-sensitivity from the syntax.

Example:
```
#ignorecase
```

### Scopes

[Scopes](scope) allow you to organize your events better and avoid ID conflicts.  
To go back to the global scope, you can use `endscope`

Example:
```
// Global scope
#scope "another scope"
// Scope "another scope"
// Events
#endscope
// Global scope
```

<h3 align="center"><a href="modules">Next</a></h3>
<h3 align="center"><a href="index">Index</a></h3>
