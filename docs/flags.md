<h3 align="center"><a href="index">Index</a></h3>

# Flags
-----

Flags are a core component of the language.  
FEL being inspired by Cave Story's [TSC language](https://www.cavestory.org/guides/basicmodding/guide/tsc.html) and seeing the power of flags has made this a core feature.

## Creating & setting flags

Creating a flag is simple; it's automatically created once you do any flag-related commands.

### FLS
Using `FLS` on a flag will set it to `true`.  
If the flag doesn't exist, it will create it and set it to `true`.

### FLU
Using `FLU`on a flag will set it to `false`.  
If the flag doesn't exist, it will create it and set it to `false`.

### FLT
Using `FLT` on a flag will set it to the opposite, if it's `true` it will set it to `false` and vice versa.  
If the flag doesn't exist, it will create it and set it to `true` (flags are initialized as false).

### FLJ
`FLJ` will create the flag that's being passed in, it will initialize it as `false` so `FLJ` with a flag that doesn't yet exist will always fail.

## Examples

Here are some very basic example of using flags in a game environment. 

### NPC dialogue
Let's say you have an NPC that gives some dialogue once you first talk to them and then says something else after that first time.
```
1>FLJ[1|2]-PRI[Hello, player! The weather's nice today, isn't it?]-FLS[1]|
2>PRI[Didn't we already speak today?]|
```

### Right-order puzzle
Let's assume we have a puzzle where the you need to interact with 3 events in the right order to proceed.  
The order is 2, 1, 3.  
When you start the puzzle, event 0 will be executed first. Then the player can interact with the 3 events.
```
0>FLS[1]-FLS[2]-FLS[3]|

1>FLU[1]-FLS[3]|
2>FLU[2]-FLS[1]-FLS[2]|
3>FLU[3]|

4>FLJ[1|5]-FLJ[2|5]-FLJ[3|5]-PRI[You did it!]|
5>PRI[Wrong order!]|
```

### Key-door scenario
Let's look at a more advanced example.  
Now the player has to find a key to open the door.  
Here's an overview of the events:
1. The event when you get the key.
2. The event that executes when you interact with the doors.

Here's an overview of the flags:
1. Whether or not the player has the key.
2. Whether or not the door is open.

Lastly, we'll assume we have a [custom command](custom-commands) in place.
* `Transfer[map_name]`: Transfers the player to another map.

```
1>PRI[You got the key!]-FLS[1]|

2>FLJ[1|3]-FLJ[2|4]-PRI[The door is closed]|
3>PRI[You open the door]-FLU[1]-FLS[2]-EXE[4]|
4>Transfer[Other area]|
```

<h3 align="center"><a href="variables">Next</a></h3>
<h3 align="center"><a href="index">Index</a></h3>
