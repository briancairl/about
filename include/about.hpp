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
namespace detail
{

/// Removes reference and const qualifiers
template <typename T> using cleaned_t = typename std::remove_const<typename std::remove_reference<T>::type>::type;

/**
 * @brief Base for attribute names
 */
struct AttrName
{};

/**
 * @brief Tagging element used to refer to member variables
 *
 * e.g. <code>MemberName<'m', 'y', '_', 'm', 'e', 'm'> == "my_mem"_member</code>
 */
template <char... Chars> struct MemberName : AttrName
{};

/**
 * @brief Tagging element used to refer to class methods
 *
 * e.g. <code>MemberName<'m', 'y', '_', 'm', 'f', 'n'> == "my_mfn"_method</code>
 */
template <char... Chars> struct MethodName : AttrName
{};

/**
 * @brief Equality comparison overload for MemberName
 *
 * @note evaluation happens entirely at compile-time, based on variadic pack
 */
template <char... Chars> constexpr bool operator==(MemberName<Chars...> lhs, MemberName<Chars...> rhs) { return true; }

/**
 * @brief Inequality comparison overload for MemberName
 *
 * @note evaluation happens entirely at compile-time, based on variadic pack
 */
template <char... Chars> constexpr bool operator!=(MemberName<Chars...> lhs, MemberName<Chars...> rhs) { return false; }

/**
 * @brief Equality comparison overload for MemberName
 *
 * @note evaluation happens entirely at compile-time, based on variadic pack
 */
template <char... LHSChars, char... RHSChars>
constexpr bool operator==(MemberName<LHSChars...> lhs, MemberName<RHSChars...> rhs)
{
  return false;
}

/**
 * @brief Inequality comparison overload for MemberName
 *
 * @note evaluation happens entirely at compile-time, based on variadic pack
 */
template <char... LHSChars, char... RHSChars>
constexpr bool operator!=(MemberName<LHSChars...> lhs, MemberName<RHSChars...> rhs)
{
  return true;
}

/**
 * @brief Equality comparison overload for MethodName
 *
 * @note evaluation happens entirely at compile-time, based on variadic pack
 */
template <char... Chars> constexpr bool operator==(MethodName<Chars...> lhs, MethodName<Chars...> rhs) { return true; }

/**
 * @brief Inequality comparison overload for MethodName
 *
 * @note evaluation happens entirely at compile-time, based on variadic pack
 */
template <char... Chars> constexpr bool operator!=(MethodName<Chars...> lhs, MethodName<Chars...> rhs) { return false; }

/**
 * @brief Equality comparison overload for MethodName
 *
 * @note evaluation happens entirely at compile-time, based on variadic pack
 */
template <char... LHSChars, char... RHSChars>
constexpr bool operator==(MethodName<LHSChars...> lhs, MethodName<RHSChars...> rhs)
{
  return false;
}

/**
 * @brief Inequality comparison overload for MethodName
 *
 * @note evaluation happens entirely at compile-time, based on variadic pack
 */
template <char... LHSChars, char... RHSChars>
constexpr bool operator!=(MethodName<LHSChars...> lhs, MethodName<RHSChars...> rhs)
{
  return true;
}

/**
 * @brief Traits type containing information about a type <code>T</code>
 *
 * For example:
 * <code>
 * // Name of the class
 * std::cout << ClassMetaInfo<T>::name << std::endl;
 *
 * // Name of a member
 * std::cout << ClassMetaInfo<T>::public_member_info<0>::name << std::endl:
 * </code>
 *
 * @tparam T  type to reflect
 */
template <typename T> struct ClassMetaInfo : std::false_type
{};

/**
 * @brief Traits type used to check if a member variable or method exists via a tag
 *
 * For example:
 * <code>
 * std::cout << std::boolalpha << ClassMemberExists<T, decltype("a"_member)>::value << std::endl;
 * std::cout << std::boolalpha << ClassMemberExists<T, decltype("b"_method)>::value << std::endl;
 * </code>
 *
 * @tparam T  type to reflect
 */
template <typename T, typename MemberTag> struct ClassMemberExists : std::false_type
{};

}  // namespace detail

//////////////////////////////////////////
//                                      //
//  CODE INTENDED FOR USES STARTS HERE  //
//                                      //
//////////////////////////////////////////

/**
 * @brief Used to check if reflection information is available for a given type
 *
 * For example:
 * <code>
 * template<typename T>
 * typename std::enable_if<has_reflection_info<T>, void>::type use_reflection(const T& value)
 * {
 *   // code which uses generated reflection traits
 * }
 * </code>
 *
 * @tparam T  type to check
 */
template <typename T>
constexpr bool has_reflection_info =
  !std::is_base_of<std::false_type, detail::ClassMetaInfo<detail::cleaned_t<T>>>::value;

/**
 * @brief Literal used to refer to a class member variable
 */
template <typename T, T... Chars> constexpr detail::MemberName<Chars...> operator""_member()
{
  return detail::MemberName<Chars...>{};
}

/**
 * @brief Literal used to refer to a class method
 */
template <typename T, T... Chars> constexpr detail::MethodName<Chars...> operator""_method()
{
  return detail::MethodName<Chars...>{};
}

/**
 * @brief Checks if a member variable exists via a tag
 *
 * For example:
 * <code>
 * std::cout << std::boolalpha << has<T>("a"_member) << std::endl;
 * </code>
 *
 * @tparam T  type to reflect
 */
template <typename ClassT, char... Chars> constexpr bool has(detail::MemberName<Chars...> _)
{
  return detail::ClassMemberExists<ClassT, detail::MemberName<Chars...>>::value;
}

/**
 * @brief Checks if a class method exists via a tag
 *
 * For example:
 * <code>
 * std::cout << std::boolalpha << has<T>("b"_method) << std::endl;
 * </code>
 *
 * @tparam T  type to reflect
 */
template <typename ClassT, char... Chars> constexpr bool has(detail::MethodName<Chars...> _)
{
  return detail::ClassMemberExists<ClassT, detail::MethodName<Chars...>>::value;
}

/**
 * @brief Returns a <code>std::tuple</code> of lvalue references to all public members, in order of declaration
 *
 * @tparam T  type to reflect
 */
template <typename T> constexpr auto get_public_members(const T& value)
{
  return detail::ClassMetaInfo<T>::public_members(value);
}

/**
 * @brief Returns a <code>std::tuple</code> of immutable lvalue references to all public members (in order of
 * declaration)
 *
 * @tparam T  type to reflect
 */
template <typename T> constexpr auto get_name(const T& value) { return detail::ClassMetaInfo<T>::name; }

/**
 * @brief <code>constexpr</code> string literal type name of a class \c T
 *
 * @tparam T  type to reflect
 */
template <typename T> constexpr const char* nameof = detail::ClassMetaInfo<T>::name;

/**
 * @brief <code>std::tuple</code> of meta information for each public class member variable, in order of declaration
 *
 * @tparam T  type to reflect
 */
template <typename T>
using public_member_info_t = typename detail::ClassMetaInfo<detail::cleaned_t<T>>::public_member_info;

}  // namespace about

#endif  // ABOUT_ABOUT_HPP