<h3 align="center"><a href="index.html">Index</a></h3>

# Loading & Executing
-----

First of all, I will assume you know how to build and compile the repo into a .dll file and how to link it to your C++ project.

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



<h3 align="center"><a href="index.html">Index</a></h3>
