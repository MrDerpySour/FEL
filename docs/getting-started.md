# FEL | Getting Started
======

## Setup | C++
-----
First of all, you build the source files as a .dll file.
Knowledge on how to build it, link it, etc, is assumed.
I use Visual Studio.

### main.cpp
```cpp
int main() {
  return 0;
}
```
Let's start off with an empty program, just to keep it simple.
Next, we'll include the `manager.hpp`, we'll be doing all our FEL-related stuff with it.

```cpp
#include <FEL/manager.hpp>
```

Next, let's create our `fel::Manager` variable.
```cpp
#include <iostream>

#include <FEL/manager.hpp>

int main() {
  fel::Manager manager;
  
  std::cin.ignore(); // So our program doesn't close immediately
}
```

We'll be going over the .fel file later, right now, let's just focus on using the `fel::Manager`.
To load a file you need to use the `load` function, it takes 1 parameter: the filepath.
```cpp
#include <iostream>

#include <FEL/manager.hpp>

int main() {
  fel::Manager manager;
  manager.load("Hello world.fel");
  
  std::cin.ignore(); // So our program doesn't close immediately
}
```
⋅⋅⋅NOTE: the file can be any extension, for the sake of clarity, we'll be using .fel for this tutorial

Now that we have loaded our file we can start executing events.
Executing events is done with the `execute` function, NOT with the `executeEvent` function.
I'll discuss the differences between the two functions later.
The `execute` function takes 1 parameter: the ID of the event to execute (int).

Let's execute event number 1.
```cpp
#include <iostream>

#include <FEL/manager.hpp>

int main() {
  fel::Manager manager;
  manager.load("Hello world.fel");
  manager.execute(1);
  
  std::cin.ignore(); // So our program doesn't close immediately
}
```
And that's the basics of loading and executing events in FEL.
