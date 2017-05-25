<h3 align="center"><a href="index.html">Index</a></h3>

# Custom Commands
------
Custom commands are where FEL really shines.  
To create a custom command you first have to inherit from the `CommandClass` class and implement the pure virtual function `execute`.

CustomCommand class:
```cpp
class CommandClass {
 public:
   virtual void execute(const std::string& parameters, FlagList* list, Context* context) = 0;
};
```

The reason I chose for a class instead of a function is because classes give you much more freedom.  
The commands from the [variables module](variables.html) are all custom commands.  

Let's take a look at an example command.  
Here we'll assume there is a `Player` class which has some public variables.  

Header file:
```cpp
class PrintStats : public fel::CustomClass {
 public:
  PrintStats(Player* player) : player_(player) {}
  
  void execute(const std::string& parameters, FlagList* list, Context* context) override;
 private:
  Player* player_ = nullptr;
}
```

Source file:
```cpp
void PrintStats::execute(const std::string& parameters, FlagList* list, Context* context) {
  printf("Player's name: %s\n", player_->name.c_str());
  
  if (player->hp > 50)
    printf("The player has more than half of his HP!\n");
}
```

Nice, our command is created.  
To add our command we need to pass it into the `fel::Manager`'s `registerFunction` function.  
`registerFunction` takes 2 parameters: the syntax and a pointer to an instance of `CommandClass`.

```cpp
fel::Manager manager;
manager.load("whatever.fel");

PrintStats print_stats(&player);

manager.registerFunction("PrintStats", &print_stats);
```

And now you're ready to use your custom command!  
It is recommended to add your functions right after you're calling `load` to avoid name-clashing with other [modules](modules.html).  

## execute VS executeEvent

In [this](loading-executing.html) tutorial I said that when executing an event, you should always use the `execute` function.  
So when do you use the executeEvent function? Inside custom commands!

If you want to execute an event inside a custom coammand, you should use use the `executeEvent` function.  
The difference?  
At the end of the `execute` function it resets the `fatal_error_` boolean, this needs to be done at the very end.  

<h3 align="center"><a href="helper.html">Helper</a></h3>
<h3 align="center"><a href="index.html">Index</a></h3>
