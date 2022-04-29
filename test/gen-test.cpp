/**
 * @copyright 2022-present About
 * @author Brian Cairl
 */

// GTest
#include <gtest/gtest.h>

// About
#include "gen-test.hpp"
#include "gen-test.hpp.about"
#include <about/common.hpp>
#include <about/utility/print.hpp>

using namespace about;

TEST(Generation, MethodExists) { ASSERT_TRUE(has<my_ns::MyClass>("my_method"_method)); }

TEST(Generation, MethodDoesNotExist) { ASSERT_FALSE(has<my_ns::MyClass>("not_my_method"_method)); }

TEST(Generation, PrintPublic)
{
  my_ns::MyClass obj{};

  std::cout << fmt<4>(obj) << std::endl;
}