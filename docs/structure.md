<h3 align="center"><a href="index.html">Index</a></h3>

# Structure
-----

### 1. Event ID

First of all, let's talk about how FEL is structured.  
An event is located on a single line.  

First of all, enter the ID of the event.  
The ID of the event is an int32 meaning that the ID can be anywhere from –2.147.483.648 to 2.147.483.647  
The only ID that can't be set is `-1`, this ID is reserved for error-checking.

For this example, let's use 1 as ID

Event so far:
```
1
```

### 2. Start marker

Next is the start marker.  
After your ID, put a `>`.  

Event so far:
```
1>
```

### 3. Syntax & Parameters

Next you can enter your [syntax](syntax.html).  
In this case, we'll be making a Hello World script.  

Looking at the syntax list we can see that `PRI` is the syntax for printing to the console.  
It takes 1 parameter: the message to print.  

To pass in parameters, you open a parameter block with the square brackets `[ ]`.  
Even if a command takes no parameters, you still open a parameter block.  
Since we're making a Hello world script, we'll pass in "Hello, world!" as the syntax.

  NOTE: Escape codes line `\n`, `\t`, `\r`, `\b`, `\v`, `\f`, `\a` and `\0` will work.

Event so far:
```
1>PRI[Hello, world!\n]
```

### 4. End marker

To end an event, append a `|`.

Event so far:
```
1>PRI[Hello, world!\n]|
```
This is a valid event and will be executed correctly.

### 5. Adding more syntax

So let's rewind to step 3 for a second.  
If you want to chain multiple commands together, we use a `-`.  

Let's say we're using a Windows system and we want to `pause` (although using the system function is bad practice).  
We can use the `SYS` command like so:

Event so far:
```
1>PRI[Hello, world!\n]-SYS[pause]
```

And now we add the `|` to end the event.  
```
1>PRI[Hello, world!\n]-SYS[pause]|
```

### Structure Philosophy

The structuring actually has some "philosophy" behind it.  
The idea was that it resembled a pipeline with the `>` being the beginning, the `-` being the pipe and the `|` being the stop.  

<h3 align="center"><a href="getting-started-cpp.html">Next</a></h3>
<h3 align="center"><a href="index.html">Index</a></h3>
