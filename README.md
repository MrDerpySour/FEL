# FEL
[![Build status](https://ci.appveyor.com/api/projects/status/6ao9lo2d8i89a2tv?svg=true)](https://ci.appveyor.com/project/Lunatoid/fel)
-----
Flag Event Language or FEL is a simple scripting language inspired by Cave Story's [TSC Language](http://www.cavestory.org/guides/basicmodding/guide/tsc.html) but aims to deliver a more customizable, flexible and easier scripting language.  

FEL is simple and has minimal control flow, it's meant to be used in conjunction with C++.    
For this reason I mostly refer to FEL as an "event language" instead of a scripting language.  

# Docs
-----
Check out the docs [here](https://lunatoid.github.io/FEL/)

# TODO
----
Here is a list of features that may or may not be implemented in the future:  

* Multiline events  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Instead of putting an end marker, there will be some kind of nextline marker.  

* Creating and importing user-made modules  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Implement a base class like `CommandClass` and register modules before loading files.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Then, at compile-time, resolve the module names and load them.

* Exporting modules  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Exporting user-made modules into some file format.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;It's a neat idea but I have no idea how to go about implementing it.

* Expanded #debug  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Seperating the current `#debug` to `#debugprint` and having #debug just turn the `debug_mode` bool on.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;The new #debug will also support new directives like `#ifdebug`, `#elsedebug` and `#enddebug`.

* Preprocessor definitions  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A new `#define` directive that will allow you to create preprocessor strings.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;This will not require the `variables` module and will be parsed at compile-time, like the reserved variable `id`.  

# DONE
-----
Here is a list of all the TODO things that I actually finished:

* Event grouping  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Grouping events together under a name, probably with a `#group "name"` and `#endgroup` directive.  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Maybe nest the current std::unordered_map into another std::unordered_map with std::string name as the key.  

## Contact
If you have any other suggestions or question, feel free to contact me.  
The best way to contact me is via [Discord](https://discordapp.com).  
My username is `Lunatoid#8100`.
