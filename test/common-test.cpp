/**
 * @copyright 2022-present About
 * @author Brian Cairl
 */

// GTest
#include <gtest/gtest.h>

// About
#include <about/common.hpp>

class TestClass
{};

namespace about
{

template <> struct ClassHas<::TestClass, decltype("cool"_method)> : std::true_type
{};

template <> struct Class<::TestClass> : std::true_type
{};

}  // namespace

using namespace about;

TEST(Common, MethodTagCompareEqual) { ASSERT_EQ("cool"_method, "cool"_method); }

TEST(Common, MethodTagCompareNotEqual) { ASSERT_NE("cool"_method, "hot"_method); }

TEST(Common, MemberTagCompareEqual) { ASSERT_EQ("cool"_method, "cool"_method); }

TEST(Common, MemberTagCompareNotEqual) { ASSERT_NE("cool"_method, "hot"_method); }

TEST(Common, HasMethod) { ASSERT_TRUE(has<::TestClass>("cool"_method)); }

TEST(Common, HasMember) { ASSERT_FALSE(has<::TestClass>("cool"_member)); }

TEST(Common, HasReflectionInfoClass) { ASSERT_TRUE((has_reflection_info<::TestClass>)); }

TEST(Common, HasReflectionInfoPrimitive) { ASSERT_FALSE((has_reflection_info<int>)); }