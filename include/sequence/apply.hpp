/**
 * @copyright 2022-present About
 * @author Brian Cairl
 */
#ifndef ABOUT_SEQUENCE_APPLY_HPP
#define ABOUT_SEQUENCE_APPLY_HPP

// C++ Standard Library
#include <tuple>
#include <type_traits>

// About
#include <about/sequence/integer_sequence.hpp>

namespace about
{
namespace detail
{

/**
 * @brief Implementation for flow::apply
 */
template <typename InvocableT, typename ArgTupleT, std::size_t... IntPack>
constexpr decltype(auto) apply(InvocableT&& fn, ArgTupleT&& targs, index_sequence<IntPack...>)
{
  static_assert(
    std::tuple_size<std::remove_reference_t<ArgTupleT>>::value == sizeof...(IntPack), "ArgTupleT size is invalid");
  return fn(std::get<IntPack>(std::forward<ArgTupleT>(targs))...);
}

}  // namespace detail

/**
 * @brief Calls function with with tuple (e.g <code>std::tuple</code>) as argument list
 *
 * @tparam InvocableT  invocable type:
 * @tparam TupleT      tuple-like type with element types order according to function argument list
 *
 * @param fn     function pointer or functor object type
 * @param targs  tuple with arguments to pass to <code>f</code>
 */
template <typename InvocableT, typename ArgTupleT> constexpr decltype(auto) apply(InvocableT&& fn, ArgTupleT&& targs)
{
  constexpr auto N = std::tuple_size<typename std::remove_reference<ArgTupleT>::type>::value;
  return detail::apply(std::forward<InvocableT>(fn), std::forward<ArgTupleT>(targs), make_index_sequence<N>{});
}

}  // namespace about

#endif  // ABOUT_SEQUENCE_APPLY_HPP
