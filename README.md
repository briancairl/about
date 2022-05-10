# about

C++11 tool for compile-time reflection utility through code generation.

Also includes some nice meta-programming utilities.

## API Documentation

Doxygen output for the **latest version** is available [here](https://briancairl.github.io/about/doxygen-out/html/index.html)

## Prerequisites

- `apt-get install castxml python`
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

enum class MyEnum
{
  VALUE_A,
  VALUE_B
};

}  // namespace my_ns
```

### Setup the build

#### Bazel

```py
load("//:bazel/about.bzl", "cc_library_with_reflection")

...

cc_library_with_reflection(
  name="test-about",
  hdrs=["test.hpp"],
  visibility=["//visibility:public"]
)
```

### Use your code and the generated compile time reflection classes

#### Basic reflection:

*Sample code:*
```c++

// C++ Standard Library
#include <iostream>
#include <type_traits>

// About
#include <about/about.hpp>

// User Code (output by "test-about")
#include "test-about.meta.hpp"

using namespace about;

template<typename T>
static typename std::enable_if<has<T>("an_int"_var)>::type print_an_int(const T& object)
{
  std::cout << "This is the value of T.an_int : " << object.an_int << std::endl;
}

template<typename T>
static typename std::enable_if<!has<T>("an_int"_var)>::type print_an_int(const T& object)
{
  std::cout << "T doesn't have an_int" << std::endl;
}

int main(int argc, char const *argv[])
{
  my_ns::MyClass obj{};
  print_an_int(obj);
  return 0;
}
```

#### Output stream overloads using `fmt`

*Sample code:*
```c++

// C++ Standard Library
#include <iostream>

// About
#include <about/utility/fmt.hpp>

// User Code (output by "test-about")
#include "test-about.meta.hpp"

int main(int argc, char const *argv[])
{
  my_ns::MyClass obj{};
  std::cout << "my_class " << about::fmt<4>(obj) << std::endl;
  return 0;
}
```

*Output:*
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

#### Output stream overloads for `enum`


*Sample code:*
```c++

// C++ Standard Library
#include <iostream>

// User Code (output by "test-about")
#include "test-about.enum_ostream.hpp"

int main(int argc, char const *argv[])
{
  std::cout << my_ns::MyEnum::VALUE_A << ", " << my_ns::MyEnum::VALUE_B << std::endl;
  return 0;
}
```

*Output:*
```
MyEnum::VALUE_A, MyEnum::VALUE_B
```
