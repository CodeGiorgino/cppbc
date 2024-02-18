# cppbc
This is a small project to port the `bc` bash command on C++

## Dependencies
To compile the library you need to install [fmtlib/fmt](https://github.com/fmtlib/fmt) (used for better log messages)

## Compilation
You can compile just by running `make` in the root directory of the project

### Make targets
Name      | Usage
:---      | :---
help      | Show the available targets
all       | Build the library
dist      | Build the library for distribution
example   | Build the library, the example code and run it
clean     | Clean build files
cleandist | Clean distribution build files

### Log trace
You can enable the log of the parsed trees by defining `LOG_TRACE` in `src/cppbc.cpp`

Example with log trace enabled:
```
============[ example.out ]=============
Source: 2 + 2

===[ Tree dump ]===
(#0)
.value = 2
.bop   = +

(#1)
.value = 2
.bop   =  

Source: 3 * 4.000000

===[ Tree dump ]===
(#0)
.value = 3
.bop   = *

(#1)
.value = 4
.bop   =  

Source: 2 + 12.000000 + 2 * 3

===[ Tree dump ]===
(#0)
.value = 2
.bop   = +

(#1)
.value = 12
.bop   = +

(#2)
.value = 2
.bop   = *

(#3)
.value = 3
.bop   =  

Result: 20
```

Example with log trace disabled:
```
============[ example.out ]=============
Result: 20
```

## Algorithm explained
When parsing the source string, a tree will be created where every node contains a value and a binary operator.
If the source contains a parentehesis contained expression, it will be evaluated first, 
then the result will be used as the value for a new node (see the previous section "Example with log trace enabled").

Multiplications and divisions will be evaluated first, then additions and subtractions will be evaluated on the collapsed tree.

## Usage
> Look at the example source code and Makefile for better understanding
Once you compiled the library, you can copy the file in `include` directory and the `cppbc.a` file into your project's directory.

Example:
```bash
git clone --depth=1 https://github.com/CodeGiorgino/cppbc.git
cd cppbc
make

cd ..
mkdir test
touch test/main.cpp
cp cppbc/include/cppbc.hpp test/
cp cppbc/cppbc.a test/
cd test
```

```cpp
/* main.cpp */

#include "./cppbc.hpp"

int main(void) {
    fmt::println("Result: {}", bc("2 + 2"));
    return 0;
}
```

```bash
g++ -Wall -Wextra -pedantic -std=c++23 main.cpp cppbc.a -o main.out && ./main.out
```
