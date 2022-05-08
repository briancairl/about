/**
 * @copyright 2022-present About
 * @author Brian Cairl
 */

// GTest
#include <gtest/gtest.h>

// About
#include "test/test_classes_with_reflection.meta.hpp"
#include <about/fmt.hpp>

using namespace about;

TEST(MetaGeneration, MethodExists) { ASSERT_TRUE(has<my_ns::MyClass>("my_method"_method)); }

TEST(MetaGeneration, MethodDoesNotExist) { ASSERT_FALSE(has<my_ns::MyClass>("not_my_method"_method)); }

TEST(MetaGeneration, PrintPublicMembers)
{
  {
    my_ns::SomethingElse obj{};
    std::cout << about::fmt<4>(obj) << std::endl;
  }

  {
    my_ns::MyClass obj{};
    std::cout << about::fmt<4>(obj) << std::endl;
  }
}

TEST(MetaGeneration, GetName)
{
  my_ns::SomethingElse obj{};
  ASSERT_EQ("SomethingElse", get_name(obj));
}

TEST(MetaGeneration, NameOfClass) { ASSERT_EQ("SomethingElse", nameof<my_ns::SomethingElse>); }

TEST(MetaGeneration, NameOfEnum) { ASSERT_EQ("MyEnum", nameof<my_ns::MyEnum>); }

TEST(MetaGeneration, NameOfNestedEnum) { ASSERT_EQ("NestedEnum", nameof<my_ns::MyClass::NestedEnum>); }
