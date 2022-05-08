/**
 * @copyright 2022-present Brian Cairl
 *
 * @file about.hpp
 */
#ifndef ABOUT_ABOUT_HPP
#define ABOUT_ABOUT_HPP

// C++ Standard Library
#include <type_traits>

namespace about
{

struct AttrName
{};

template <char... Chars> struct MemberName : AttrName
{};

template <char... Chars> struct MethodName : AttrName
{};

template <typename T, T... Chars> constexpr MemberName<Chars...> operator""_member() { return MemberName<Chars...>{}; }

template <typename T, T... Chars> constexpr MethodName<Chars...> operator""_method() { return MethodName<Chars...>{}; }

template <char... Chars> constexpr bool operator==(MemberName<Chars...> lhs, MemberName<Chars...> rhs) { return true; }

template <char... Chars> constexpr bool operator!=(MemberName<Chars...> lhs, MemberName<Chars...> rhs) { return false; }

template <char... LHSChars, char... RHSChars>
constexpr bool operator==(MemberName<LHSChars...> lhs, MemberName<RHSChars...> rhs)
{
  return false;
}

template <char... LHSChars, char... RHSChars>
constexpr bool operator!=(MemberName<LHSChars...> lhs, MemberName<RHSChars...> rhs)
{
  return true;
}

template <char... Chars> constexpr bool operator==(MethodName<Chars...> lhs, MethodName<Chars...> rhs) { return true; }

template <char... Chars> constexpr bool operator!=(MethodName<Chars...> lhs, MethodName<Chars...> rhs) { return false; }


template <char... LHSChars, char... RHSChars>
constexpr bool operator==(MethodName<LHSChars...> lhs, MethodName<RHSChars...> rhs)
{
  return false;
}

template <char... LHSChars, char... RHSChars>
constexpr bool operator!=(MethodName<LHSChars...> lhs, MethodName<RHSChars...> rhs)
{
  return true;
}

template <typename T> struct ClassMetaInfo : std::false_type
{};

template <typename T, typename MemberTag> struct ClassMemberExists : std::false_type
{};

template <typename ClassT, char... Chars> constexpr bool has(MethodName<Chars...> _)
{
  return ClassMemberExists<ClassT, MethodName<Chars...>>::value;
}

template <typename ClassT, char... Chars> constexpr bool has(MemberName<Chars...> _)
{
  return ClassMemberExists<ClassT, MemberName<Chars...>>::value;
}

template <typename T> constexpr auto get_public_members(const T& value)
{
  return ClassMetaInfo<T>::public_members(value);
}

template <typename T> using cleaned_t = typename std::remove_const<typename std::remove_reference<T>::type>::type;

template <typename T>
constexpr bool has_reflection_info = !std::is_base_of<std::false_type, ClassMetaInfo<cleaned_t<T>>>::value;

template <typename T> using public_member_info_t = typename ClassMetaInfo<cleaned_t<T>>::public_member_info;

}  // namespace about

#endif  // ABOUT_ABOUT_HPP