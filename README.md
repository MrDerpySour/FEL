
<p align="center"><img src="https://raw.githubusercontent.com/Lunatoid/FEL/master/docs/FEL-badge-dark.png" style="max-width:100%; height:auto;" alt="Logo"/></p>

<a href="https://ci.appveyor.com/project/Lunatoid/fel"><p align="center"><img src="https://ci.appveyor.com/api/projects/status/40a9g0j0uskbx1f1?svg=true" href="https://ci.appveyor.com/project/Lunatoid/fel" alt="Build status"/> [![Codacy Badge](https://api.codacy.com/project/badge/Grade/be9c32cac71145c3af905b0f0c692ae6)](https://www.codacy.com/app/lunatoid.contact/FEL?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=Lunatoid/FEL&amp;utm_campaign=Badge_Grade)</p></a>

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

* Multiline events  
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Instead of putting an end marker, there will be some kind of nextline marker.  

## Contact
Contact info can be found [here](https://lunatoid.github.io/)
