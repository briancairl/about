/**
 * @copyright 2022-present About
 * @author Brian Cairl
 */
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
