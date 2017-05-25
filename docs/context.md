<h3 align="center"><a href="index.html">Index</a></h3>

# Context
-----

The `context` class is a very powerful class and will give you all the info you need on the current FEL execution.  
You can get a pointer to the `context` by using the `context` function on the `fel::Manager`.  

Here is a list of what the context allows you to do:
* Parse strings containing variables
* Check if [#debug](directives.html) has been called
* Check if [#ingorecase](directives.html) has been called
* Get a list of all [#link](directives.html)ed files
* Get a list of all loaded [#module](directives.html)s
* Get the [#flags](directives.html) filepath
* Get the [#vars](directives.html) filepath
* Access the [variables](variables.html) manager
* Manipulate the current instructions
* Manipulate the current instruction index

<h3 align="center"><a href="custom-commands.html">Next</a></h3>
<h3 align="center"><a href="index.html">Index</a></h3>
