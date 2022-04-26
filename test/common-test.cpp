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

template <> struct Has<::TestClass, decltype("cool"_method)> : std::true_type
{};

}  // namespace

using namespace about;

TEST(Common, MethodTagCompareEqual) { ASSERT_EQ("cool"_method, "cool"_method); }

TEST(Common, MethodTagCompareNotEqual) { ASSERT_NE("cool"_method, "hot"_method); }

TEST(Common, MemberTagCompareEqual) { ASSERT_EQ("cool"_method, "cool"_method); }

TEST(Common, MemberTagCompareNotEqual) { ASSERT_NE("cool"_method, "hot"_method); }

TEST(Common, HasMethod) { ASSERT_TRUE(has<::TestClass>("cool"_method)); }

TEST(Common, HasMember) { ASSERT_FALSE(has<::TestClass>("cool"_member)); }