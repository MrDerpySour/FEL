<h3 align="center"><a href="index.html">Index</a></h3>

# Miscellaneous
-----

## Comments
FEL supports both singleline and multiline comments.

Example:
```
// Singleline comment

/*
Multi
line
comment
*/
```
  NOTE: make sure there is no whitespace before or after the comment
  
## Reserved variables
There are only 2 reserved variables, `id` and `scope`.
`id` is automatically converted to the event's id.  
`scope` is automatically converted to the event's scope.  

Reserved variables are accessed the same way as other variables, with 2 `%` on each side.  
You do not need to include the `variables` [module](modules.html) for this to work.

Example:
```
1>PRI[This is event id '%id%']|

11321312>PRI[This is event id '%id%']|
```
While normal variables are parsed at runtime, `id` will be parsed at compile-time.

## Math
Parameters which take a numeral value will support math.  
+, -, *, /, % are the supported operations.  
It keeps in mind the correct order and will also give math in between parentheses higher priority.  

Example:
```
1>EXE[%id% + 1]|
2>REG[f|float|5]-SET[f|%f% * %f%]-EXE[%f%]|

25>PRI[Hello!]|
```

## FEL Variable Table
FEL Variable Table or fvt is the format variables are saved in.  
The format is pretty simple, it starts with a `>` followed by the name, type and value with `|` in between and finally: a `<`.  

Example:
```
>hello|string|Hello, world!<
>number|float|3.1415<
```

<h3 align="center"><a href="index.html">Index</a></h3>
