<h3 align="center"><a href="index">Index</a></h3>

# Context
-----

The `context` class is a very powerful class and will give you all the info you need on the current FEL execution.  
You can get a pointer to the `context` by using the `context` function on the `fel::Manager`.  

Here is a list of what the context allows you to do:
* Parse strings containing variables
* Check if [#debug](directives) has been called
* Check if [#ingorecase](directives) has been called
* Get a list of all [#link](directives)ed files
* Get a list of all loaded [#module](directives)s
* Get the [#flags](directives) filepath
* Get the [#vars](directives) filepath
* Access the [variables](variables)
* Manipulate the current instructions
* Manipulate the current instruction index

## Print

The context's `print` function is where all the text output in FEL is done.  
The print method is a wrapper function that invokes the `std::function`.  
The default print function is set to a lambda function that uses `std::cout`.  

Example:
```cpp
fel::Manager manager;

std::ofstream of("output.txt");

// With this function all output will be put into "output.txt" and printed on the screen
manager.context()->setPrintFunc([&](std::string str) {
  of << str;
  std::cout << str;
});

manager.load("file.fel");

// ...
```

<h3 align="center"><a href="custom-commands">Next</a></h3>
<h3 align="center"><a href="index">Index</a></h3>
