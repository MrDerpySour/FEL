<h3 align="center"><a href="index">Index</a></h3>

# Scope
-----

If you want to organize your code you can use scopes.  
You can define a scope by using the [directive](directives) `scope` followed with a name.  
After you put your events inside the group you can type `endscope` to go back to the global scope.  

Nested scopes are not supported.

Example:
```
// Global scope
#scope "hello"
// Scope "hello"
#endscope
// Global scope
```

Example of nested behaviour:
```
// Global scope
#scope "1"
// Scope "1"
#scope "2"
// Scope "2"
#endscope
// Global scope
#endscope
// Still global scope
```

If you use any commands that execute events ([`EXE`](syntax), [`FLJ`](syntax)m or [`RPT`](syntax)) you can use the optional parameter `scope`.  
If the argument is not set, global scope will be assumed. 

<h3 align="center"><a href="fel-misc">Next</a></h3>
<h3 align="center"><a href="index">Index</a></h3>
