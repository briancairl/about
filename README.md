# about

C++11 tool for compile-time reflection utility through code generation.

Also includes some nice meta-programming utilities.

## Prerequisites

- `apt-get install castxml`
- `pip install pygccxml`
- `bazel` or `cmake`

## Supported Build Systems

- `bazel`
- `cmake` [in the future...]

## How To

### Write some code you want reflect


```c++
// test.hpp

#pragma once

namespace my_ns
{

struct Something
{
  float real_number;
};

struct SomethingElse
{
  Something a, b;
};

class MyClass
{
public:
  int a;
  float b;
  double c;

  SomethingElse d;

  void my_method(const char* my_arg);

private:
  const char* privates = "don't touch me";
};

}  // namespace my_ns
```

### Setup the build

#### Bazel

```py
load("//:bazel/about.bzl", "cc_library_about")

...

cc_library_about(
  name="test-about",
  hdrs=["test.hpp"],
  visibility=["//visibility:public"]
)
```

### Use your code and the generated compile time reflection classes

```c++

// C++ Standard Library
#include <iostream>

// About
#include <about/utility/print.hpp>

// My Code
#include "test-about.hpp"

int main(int argc, char const *argv[])
{
  my_ns::MyClass obj{};
  std::cout << "my_class " << about::fmt<4>(obj) << std::endl;
  return 0;
}
```

#### Output
```
my_class {
   "a" : 0,
   "b" : 0,
   "c" : 0,
   "d" : {
       "a" : {
           "real_number" : 0
       },
       "b" : {
           "real_number" : 0
       }
   }
}
```
