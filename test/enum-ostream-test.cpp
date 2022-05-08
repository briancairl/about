/**
 * @copyright 2022-present About
 * @author Brian Cairl
 */

// C++ Standard Library
#include <sstream>

// GTest
#include <gtest/gtest.h>

// About
#include "test/test_classes_with_reflection.enum_ostream.hpp"

using namespace about;

TEST(EnumOstreamGeneration, ValidMember)
{
  {
    std::ostringstream oss;
    oss << my_ns::MyEnum::THIS;
    ASSERT_EQ(oss.str(), "MyEnum::THIS");
  }

  {
    std::ostringstream oss;
    oss << my_ns::MyEnum::CODE;
    ASSERT_EQ(oss.str(), "MyEnum::CODE");
  }
}

TEST(EnumOstreamGeneration, InvalidMember)
{
  {
    std::ostringstream oss;
    oss << static_cast<my_ns::MyEnum>(-1);
    ASSERT_EQ(oss.str(), "INVALID <<MyEnum>>");
  }
}
