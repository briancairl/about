/**
 * @copyright 2022-present About
 * @author Brian Cairl
 */
#ifndef ABOUT_SEQUENCE_FOR_EACH_HPP
#define ABOUT_SEQUENCE_FOR_EACH_HPP

// C++ Standard Library
#include <tuple>
#include <type_traits>

// About
#include <about/sequence/apply.hpp>
#include <about/sequence/integer_sequence.hpp>

namespace about
{

/**
 * @brief Invokes a callback, \c cb, for elements of each tuple, \c tups
 *
 *        Callback should take N args, where N is \c sizeof...(tups)
 *        @code{.cpp}
 *        for_each(
 *          [](const auto& tuple1_element, const auto& tuple2_element, ..., const auto& tupleN_element)
 *          {
 *          },
 *          tuple1,
 *          tuple2,
 *          ...,
 *          tupleN
 *        );
 *        @endcode
 */
template <typename CallbackT, typename... TupleTs> void for_each(CallbackT&& cb, TupleTs&&... tups);

template <std::size_t I, std::size_t N> struct Enumeration
{};

/**
 * @brief Invokes a callback, \c cb, for elements of each tuple, \c tups
 *
 *        Callback should take N args, where N is \c sizeof...(tups)
 *        @code{.cpp}
 *        for_each(
 *          [](const auto& tuple1_element, const auto& tuple2_element, ..., const auto& tupleN_element)
 *          {
 *          },
 *          tuple1,
 *          tuple2,
 *          ...,
 *          tupleN
 *        );
 *        @endcode
 */
template <typename CallbackT, typename... TupleTs> void for_each_enumerated(CallbackT&& cb, TupleTs&&... tups);

namespace detail
{

/**
 * @brief Generates (and statically checks) common size between tuples, \c TupleTs
 */
template <typename... TupleTs> struct CommonTupleSize;

/**
 * @copydoc CommonTupleSize
 *
 *          Termination case
 */
template <typename FirstTupleT> struct CommonTupleSize<FirstTupleT>
{
  static constexpr std::size_t size = std::tuple_size<std::remove_reference_t<FirstTupleT>>::value;
};

/**
 * @copydoc CommonTupleSize
 *
 *          Size-checking case
 */
template <typename FirstTupleT, typename SecondTupleT, typename... OtherTupleTs>
struct CommonTupleSize<FirstTupleT, SecondTupleT, OtherTupleTs...>
{
  static constexpr std::size_t size = std::tuple_size<std::remove_reference_t<FirstTupleT>>::value;
  static_assert(size == CommonTupleSize<SecondTupleT, OtherTupleTs...>::size, "tuple sizes do not match");
};

/**
 * @brief Calls \c about::apply on a tuple formed of elements at each \c TupleTs at \c Index
 */
template <std::size_t Index> struct MultiTupleForEachAdapter
{
  template <typename CallbackT, typename... TupleTs> inline static void exec(CallbackT&& cb, TupleTs&&... tups)
  {
    about::apply(std::forward<CallbackT>(cb), std::forward_as_tuple(std::get<Index>(std::forward<TupleTs>(tups))...));
  }
};

/**
 * @copydoc for_each
 * @warn implementation
 */
template <typename CallbackT, std::size_t... Indices, typename... TupleTs>
inline void for_each(CallbackT&& cb, index_sequence<Indices...> _, TupleTs&&... tups)
{
  [[maybe_unused]] const auto __list = std::initializer_list<int>{
    0,
    (MultiTupleForEachAdapter<Indices>::template exec(std::forward<CallbackT>(cb), std::forward<TupleTs>(tups)...),
     1)...};
}

/**
 * @copydoc for_each
 * @warn implementation
 */
template <typename CallbackT, std::size_t... Indices, typename... TupleTs>
inline void for_each_enumerated(CallbackT&& cb, index_sequence<Indices...> _, TupleTs&&... tups)
{
  static constexpr std::size_t Len = sizeof...(Indices);
  [[maybe_unused]] const auto __list =
    std::initializer_list<int>{0,
                               (MultiTupleForEachAdapter<Indices>::template exec(
                                  std::forward<CallbackT>(cb),
                                  std::forward_as_tuple(Enumeration<Indices, Len>{}...),
                                  std::forward<TupleTs>(tups)...),
                                1)...};
}

}  // namespace detail

/**
 * @copydoc for_each
 * @warn implementation
 */
template <typename CallbackT, typename... TupleTs> inline void for_each(CallbackT&& cb, TupleTs&&... tups)
{
  static const std::size_t IterationCount = detail::CommonTupleSize<TupleTs...>::size;
  detail::for_each(std::forward<CallbackT>(cb), make_index_sequence<IterationCount>{}, std::forward<TupleTs>(tups)...);
}

/**
 * @copydoc for_each
 * @warn implementation
 */
template <typename CallbackT, typename... TupleTs> inline void for_each_enumerated(CallbackT&& cb, TupleTs&&... tups)
{
  static const std::size_t IterationCount = detail::CommonTupleSize<TupleTs...>::size;
  detail::for_each_enumerated(
    std::forward<CallbackT>(cb), make_index_sequence<IterationCount>{}, std::forward<TupleTs>(tups)...);
}

}  // namespace about

#endif  // ABOUT_SEQUENCE_FOR_EACH_HPP
