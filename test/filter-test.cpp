/**
 * @copyright 2022-present About
 * @author Brian Cairl
 */

// C++ Standard Library
#include <tuple>

// GTest
#include <gtest/gtest.h>

// About
#include "test/test_classes_with_reflection.meta.hpp"
#include <about/filter.hpp>
#include <about/for_each.hpp>

using namespace about;

template <typename T> struct HasA : std::integral_constant<bool, has<T>("a"_var)>
{};

TEST(Merge, TupleMergeNonEmpty)
{
  using tup = std::tuple<int>;
  ASSERT_EQ((std::tuple_size<tup>::value), 1UL);

  using tup_merged = merge_t<float, tup>;
  ASSERT_EQ((std::tuple_size<tup_merged>::value), 2UL);
}

TEST(Merge, TupleMergeEmpty)
{
  using tup = std::tuple<>;
  ASSERT_EQ((std::tuple_size<tup>::value), 0UL);

  using tup_merged = merge_t<float, tup>;
  ASSERT_EQ((std::tuple_size<tup_merged>::value), 1UL);
}

TEST(Filter, TupleFilterNonEmpty)
{
  using tup = std::tuple<int, float, my_ns::SomethingElse, double, my_ns::MyClass>;
  ASSERT_EQ((std::tuple_size<tup>::value), 5UL);

  using filtered_tup = filter_t<HasA, tup>;
  ASSERT_EQ((std::tuple_size<filtered_tup>::value), 2UL);
}

TEST(Filter, TupleEmpty)
{
  using tup = std::tuple<>;
  ASSERT_EQ((std::tuple_size<tup>::value), 0UL);

  using filtered_tup = filter_t<HasA, tup>;
  ASSERT_EQ((std::tuple_size<filtered_tup>::value), 0UL);
}
