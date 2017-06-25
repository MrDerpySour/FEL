<h3 align="center"><a href="index">Index</a></h3>

# Loading & Executing
-----

First of all, I will assume you know how to compile the repository into a static library (.lib) and link it to your C++ project.

Let's start with a blank `main.cpp` file.
```cpp
int main() {
  return 0;
}
```

Now we'll include `manager.hpp`, we'll be doing all our FEL-based operations with the manager.
Let's also make our `fel::Manager` variable.

```cpp
#include <FEL/manager.hpp>

int main() {
  fel::Manager manager;
  return 0;
}
```

Let's take our Hello World script from [this](structure) tutorial and load it.  
We can load files using the `load` function, which takes 1 parameter: the filepath.

```cpp
#include <FEL/manager.hpp>

int main() {
  fel::Manager manager;
  manager.load("Hello world.fel");
  return 0;
}
```

`load` will return false if there were any errors encountered, it will also print the errors to the screen.  
Now to execute an event we can use the `execute` function, which takes 2 parameters: the event's id and the [scope](scope).

```cpp
#include <FEL/manager.hpp>

int main() {
  fel::Manager manager;
  manager.load("Hello world.fel");
  manager.execute(1);  
  return 0;
}
```
NOTE: use the `execute` function and NOT the `executeEvent` function. [See why.](custom-commands)

To `execute` events that aren't in the global scope, simply use the second parameter to specify a scope.  

```cpp
#include <FEL/manager.hpp>

int main() {
  fel::Manager manager;
  manager.load("Hello, world.fel");
  manager.execute(1, "subscope");
  return 0;
}
```

These are the basics of loading and executing events.

<h3 align="center"><a href="context">Next</a></h3>
<h3 align="center"><a href="index">Index</a></h3>
