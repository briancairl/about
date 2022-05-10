/**
 * @copyright 2022-present About
 * @author Brian Cairl
 */
#ifndef ABOUT_FILTER_HPP
#define ABOUT_FILTER_HPP

// C++ Standard Library
#include <tuple>
#include <type_traits>

// About
#include <about/merge.hpp>

namespace about
{
#ifndef DOXYGEN_SKIP
namespace detail
{

template <template <typename> class Filter, typename TupT> struct FilterImpl
{
  using type = TupT;
};

template <template <typename> class Filter, template <typename...> class TupTmpl, typename T>
struct FilterImpl<Filter, TupTmpl<T>>
{
  using type = typename std::conditional<Filter<T>::value, TupTmpl<T>, TupTmpl<>>::type;
};

template <
  template <typename>
  class Filter,
  template <typename...>
  class TupTmpl,
  typename T0,
  typename T1,
  typename... OtherTs>
struct FilterImpl<Filter, TupTmpl<T0, T1, OtherTs...>>
{
  using type = typename std::conditional<
    Filter<T0>::value,
    merge_t<T0, typename FilterImpl<Filter, TupTmpl<T1, OtherTs...>>::type>,
    typename FilterImpl<Filter, TupTmpl<T1, OtherTs...>>::type>::type;
};

}  // namespace detail
#endif  // DOXYGEN_SKIP

template <template <typename> class Filter, typename TupT>
using filter_t = typename detail::FilterImpl<Filter, TupT>::type;

}  // namespace about

#endif  // ABOUT_FILTER_HPP
