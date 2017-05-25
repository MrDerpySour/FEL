<h3 align="center"><a href="index.html">Index</a></h3>

# Variables
-----

Variables are part of the `variables` [module](modules.html).  

## Creating variables
Creating variables is different than [creating flags](flags.html).  
Unlike flags, variables aren't automatically created, you need to use the `REG` command.  
If you look at the [syntax](syntax.html) you can see that it takes 3 parameters.  
1. The name
2. The type
3. The value

Example:
```
1>REG[hello|string|Hello, world!]|
```

This will create a variable named `hello` and give it the value "Hello, world!".  

## Inserting variables
You can insert variables by surrounding it with 2 `%`.  
This can be done with any parameter field.  

Let's print our "Hello World!" with `PRI`
```
1>REG[hello|string|Hello, world!]-PRI[%hello%]|
```

## Naming limits
There are a few limits.  
First of all, you can't have `%` in variable names, at one point it would be ignored as empty variables.
Secondly, you can't name your variable `id`, as this is a [reserved variable](fel-misc.html).

## Getting meta
Since you can insert variables into any field, it can get pretty meta.  

Example:
```
1>REG[type|string|string]-REG[1|%type%|hello]-SET[type|float]-REG[2|%type%|2.5]|
```

<h3 align="center"><a href="fvt.html">Next</a></h3>
<h3 align="center"><a href="index.html">Index</a></h3>
