
namespace test
{

class Something
{
  float real_number;
};

/**
 * Very cool
 */
class MyClass
{
public:
  /**
   * Very cool
   */
  struct Inner
  {
    float eeee;
    float* p;

    Inner& operator=(const Inner&) = delete;
  };

  int a, b, d;

  Something real;

  int func();

private:
  float e;
};

template <typename T> class MyClassTemplate
{
  int a, b, c;
};

}  // namespace test