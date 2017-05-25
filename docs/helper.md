<h3 align="center"><a href="index.html">Index</a></h3>

# Helper
-----

The `helper.hpp` file contains a bunch of nice helper functions.  
Let's go over them.

### tokenize
Probably the most used function. It splits a string into a vector of strings by the `seperator` char.  
It takes a string `str` to tokenize, a `seperator` char and a bool whether or not it should delete the empty entries.

Example:
```cpp
std::string str = "Hello|Bye";
std::vector<std::string> tokenized = fel::helper::tokenize(str, '|', true);

/*
Result:
Hello
Bye
*/
```

### str_replace_all
This function `find`s and `replace`s all occurances in a string.  
It takes a string `str`, a string `find` and a string `replace`.

```cpp
std::string str = "HelloXGithubX!";

fel::helper::str_replace_all(str, "X", " ");

/*
Result:
"Hello Github!"
*/

// OR

std::string str = "HelloXGithubX!";

std::string str2 = fel::helper::str_replace_all(str, "X", " ");

/*
Now both str and str2 will be set to "Hello Github!"
*/
```

### createFile
Creates a file at the specified `file_path` and writes the file with the specified `contents`.  

Example:
```cpp
bool result = fel::helper::createFile("Hello.txt", "Hello universe!");

if (result == false)
  printf("Something went wrong!");
```

### getInt
This will return a randomly generated int within the range set.

Example:
```cpp
int dice_result = fel::helper::getInt(1, 6);
```

### parseMathString
This will parse a string containing a math expression

Example:
```cpp
doulbe result = fel::helper::parseMathString("(((1+7)*6)+3)%4*4");
/*
Result:
12.000000
*/
```

<h3 align="center"><a href="index.html">Index</a></h3>
