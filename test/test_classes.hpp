/**
 * @copyright 2022-present About
 * @author Brian Cairl
 */

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
  typedef int typdef_member_type;
  typedef float typdef_member_type_2;

  int a;
  float b;
  double c;

  SomethingElse d;

  void my_method(const char* my_arg);

  // A nested enumeration
  enum class NestedEnum
  {
    PASS,
    FAIL,
    NOOP
  };

private:
  const char* privates = "don't touch me";
};

enum class MyEnum
{
  THIS,
  IS,
  A,
  CODE
};

}  // namespace my_ns
