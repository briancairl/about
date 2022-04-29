/**
 * @copyright 2022-present Brian Cairl
 *
 * @file print.hpp
 */
#ifndef ABOUT_UTILITY_PRINT_HPP
#define ABOUT_UTILITY_PRINT_HPP

// C++ Standard Library
#include <ostream>
#include <type_traits>
#include <utility>

// About
#include <about/common.hpp>
#include <about/sequence/for_each.hpp>

namespace about
{
namespace detail
{

template <std::size_t Step> class Printer
{
public:
  Printer(std::ostream& os, const std::size_t justification) : os_{std::addressof(os)}, justification_{justification} {}

  template <std::size_t I, std::size_t N, typename NameT, typename ValueT>
  void operator()(Enumeration<I, N> e, NameT _, ValueT&& v) const;

private:
  std::ostream* os_;
  std::size_t justification_;
};

template <std::size_t Step = 2UL, typename ValueT>
typename std::enable_if<!has_reflection_info<ValueT>>::type
print(std::ostream& os, ValueT&& value, const std::size_t justification = Step)
{
  os << std::forward<ValueT>(value);
}

template <std::size_t Step = 2UL, typename ValueT>
typename std::enable_if<has_reflection_info<ValueT>>::type
print(std::ostream& os, ValueT&& value, const std::size_t justification = Step)
{
  os << "{\n";
  for_each_enumerated(
    detail::Printer<Step>{os, justification},
    get_public_member_names(std::forward<ValueT>(value)),
    get_public_members(std::forward<ValueT>(value)));
  os << "\n";
  os << std::setw(justification - Step) << '}';
}

template <std::size_t I, std::size_t N> constexpr bool put_comma_condition = (I + 1 < N);

template <std::size_t I, std::size_t N>
typename std::enable_if<put_comma_condition<I, N>>::type put_comma(std::ostream& os)
{
  os << ",\n";
}

template <std::size_t I, std::size_t N>
typename std::enable_if<!put_comma_condition<I, N>>::type put_comma(std::ostream& os)
{}

template <std::size_t Step>
template <std::size_t I, std::size_t N, typename NameT, typename ValueT>
void Printer<Step>::operator()(Enumeration<I, N> e, NameT _, ValueT&& v) const
{
  (*os_) << std::setw(justification_) << '"' << NameT::name << "\" : ";
  print<Step>(*os_, std::forward<ValueT>(v), this->justification_ + Step);
  put_comma<I, N>(*os_);
}

template <std::size_t N, typename T> struct Formatted
{
  Formatted(const T& _v) : v(_v) {}
  const T& v;
};

}  // namespace detail

template <std::size_t N = 4UL, typename T> detail::Formatted<N, T> fmt(const T& value)
{
  return detail::Formatted<N, T>{value};
}

template <std::size_t N, typename T>
std::ostream& operator<<(std::ostream& os, const detail::Formatted<N, T>& print_wrapper)
{
  detail::print<N>(os, print_wrapper.v);
  return os;
}

}  // namespace about

#endif  // ABOUT_UTILITY_PRINT_HPP