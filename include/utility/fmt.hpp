/**
 * @copyright 2022-present Brian Cairl
 *
 * @file fmt.hpp
 */
#ifndef ABOUT_FMT_HPP
#define ABOUT_FMT_HPP

// C++ Standard Library
#include <ostream>
#include <type_traits>
#include <utility>

// About
#include <about/about.hpp>
#include <about/for_each.hpp>

namespace about
{
namespace detail
{

template <std::size_t Justification> class Printer
{
public:
  Printer(std::ostream& os, const std::size_t justification) : os_{std::addressof(os)}, justification_{justification} {}

  template <std::size_t I, std::size_t N, typename NameT, typename ValueT>
  void operator()(Enumeration<I, N> e, NameT _, ValueT&& v) const;

private:
  std::ostream* os_;
  std::size_t justification_;
};

template <std::size_t Justification = 2UL, typename ValueT>
typename std::enable_if<!has_reflection_info<ValueT>>::type
fmt_print(std::ostream& os, ValueT&& value, const std::size_t justification = Justification)
{
  os << std::forward<ValueT>(value);
}

template <std::size_t Justification = 2UL, typename ValueT>
typename std::enable_if<has_reflection_info<ValueT>>::type
fmt_print(std::ostream& os, ValueT&& value, const std::size_t justification = Justification)
{
  os << "{\n";
  ::about::for_each_enumerated(
    ::about::detail::Printer<Justification>{os, justification},
    ::about::public_member_info_t<ValueT>{},
    ::about::get_public_members(std::forward<ValueT>(value)));
  os << "\n";
  os << std::setw(justification - Justification) << '}';
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

template <std::size_t Justification>
template <std::size_t I, std::size_t N, typename NameT, typename ValueT>
void Printer<Justification>::operator()(Enumeration<I, N> e, NameT _, ValueT&& v) const
{
  (*os_) << std::setw(justification_) << '"' << NameT::name << "\" : ";
  fmt_print<Justification>(*os_, std::forward<ValueT>(v), this->justification_ + Justification);
  put_comma<I, N>(*os_);
}

template <std::size_t N, typename T> struct Formatted
{
  constexpr Formatted(const T& _v) : v(_v) {}
  const T& v;
};

}  // namespace detail

/**
 * @brief Wraps object in wrapper used to apply formatted \c ostream serialziation
 */
template <std::size_t N = 4UL, typename T> constexpr detail::Formatted<N, T> fmt(const T& value)
{
  return detail::Formatted<N, T>{value};
}

/**
 * @brief Auto-formatted <code>ostream</code> overload
 *
 * @param[in/out] os  ostream
 * @param fmt_print_wrapper  wrapper around object of type <code>T</code> used to designate formatted serialization
 *
 * @return \c os
 */
template <std::size_t N, typename T>
std::ostream& operator<<(std::ostream& os, const detail::Formatted<N, T>& fmt_print_wrapper)
{
  detail::fmt_print<N>(os, fmt_print_wrapper.v);
  return os;
}

}  // namespace about

#endif  // ABOUT_FMT_HPP