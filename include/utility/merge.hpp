/**
 * @copyright 2022-present About
 * @author Brian Cairl
 */
#ifndef ABOUT_MERGE_HPP
#define ABOUT_MERGE_HPP

namespace about
{
#ifndef DOXYGEN_SKIP
namespace detail
{

template <typename T, typename TupT> struct MergeImpl;

template <typename T, template <typename...> class TupTmpl, typename... OriginaTs>
struct MergeImpl<T, TupTmpl<OriginaTs...>>
{
  using type = TupTmpl<T, OriginaTs...>;
};

}  // namespace detail
#endif  // DOXYGEN_SKIP

template <typename T, typename TupT> using merge_t = typename detail::MergeImpl<T, TupT>::type;

}  // namespace about

#endif  // ABOUT_MERGE_HPP
