<h3 align="center"><a href="index.html">Index</a></h3>

# Scope
-----

If you want to organize your code you can use scopes.  
You can define a scope by using the [directive](directives.html) `group` followed with a name.  
After you put your events inside the group you can type `endgroup` to go back to the global scope.  

Nested scopes are not supported.

Example:
```
// Global scope
#group "hello"
// Scope "hello"
#endgroup
// Global scope
```

Example of nested behaviour:
```
// Global scope
#group "1"
// Scope "1"
#group "2"
// Scope "2"
#endgroup
// Global scope
#endgroup
// Still global scope
```

If you use any commands that execute events ([`EXE`](syntax.html), [`FLJ`](syntax.html)m or [`RPT`](syntax.html)) you can use the optional parameter `scope`.  
If the argument is not set, global scope will be assumed. 

<h3 align="center"><a href="fel-misc.html">Next</a></h3>
<h3 align="center"><a href="index.html">Index</a></h3>
