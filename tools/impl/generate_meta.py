#!/bin/python

# Standard Library
import os
from typing import (Dict, List, Optional)

# PyGCCXML
from pygccxml import utils
from pygccxml import declarations
from pygccxml import parser as xml_parser

# About
from impl.common import open_output_handle

START_OF_FILE = """
/**
 * THIS CODE WAS AUTO-GENERATED
 */
#ifndef {gaurd}__META_HPP
#define {gaurd}__META_HPP


// C++ Standard Library
#include <type_traits>
#include <utility>

// About
#include <about/about.hpp>

// USER LIBRARIES

"""

END_OF_FILE = """
#endif // {gaurd}__META_HPP
"""


def expand_enum(out, ns_name:str, decl):
    fully_qualified_enum_name = f"{ns_name}::{decl.name}"
    out.write(f"""
template<>
struct ClassMetaInfo<{ns_name}::{decl.name}>
{{
    // Enum name as string literal
    static constexpr const char* name = \"{decl.name}\";
}};
""")


def expand_class(out, ns_name:str, decl):
    _public_members_comma_sep = ', '.join([f"v.{v.name}" for v in decl.public_members if isinstance(v, declarations.variable_t)])
    out.write(f"""
template<>
struct ClassMetaInfo<{ns_name}::{decl.name}>
{{
    // Class name as string literal
    static constexpr const char* name = \"{decl.name}\";
""")

    member_name_wrappers = []
    for v in decl.public_members:
        if isinstance(v, declarations.variable_t):
            member_name_wrappers.append(f"MemberInfo__{decl.name}__{v.name}")
            if isinstance(v._decl_type, declarations.declarated_t):
                var_type_name = f"{ns_name}::{v.decl_type.declaration.name}"
            else:
                var_type_name = v.decl_type._name
            out.write(f"""
struct MemberInfo__{decl.name}__{v.name}
{{
    using type = {var_type_name};
    static constexpr const char* name = "{v.name}";
}};
""")

    out.write(f"""
/**
 * @brief Sequence containing information for all public members
 */
using public_member_info = ::std::tuple<{", ".join(member_name_wrappers)}>;

/**
 * @brief Returns tuple of references to all public members
 */
static constexpr decltype(auto) public_members({ns_name}::{decl.name}& v)
{{
    return ::std::tie({_public_members_comma_sep});
}}

/**
 * @brief Returns tuple of const references to all public members
 */
static constexpr decltype(auto) public_members(const {ns_name}::{decl.name}& v)
{{
    return ::std::tie({_public_members_comma_sep});
}}
""")

    out.write(f"""
}};  // class ClassMetaInfo<{ns_name}::{decl.name}>
""")

    for mem in decl.public_members:
        if isinstance(mem, declarations.variable_t):
            out.write(f"""
/**
 * @brief Checks if class has a public member variable <code>{mem.name}</code>
 */
template<>
struct ClassMemberExists<{ns_name}::{decl.name}, decltype("{mem.name}"_member)> : std::true_type {{}};
""")
        elif isinstance(mem, declarations.member_function_t) or isinstance(mem, declarations.member_operator_t):
            out.write(f"""
/**
 * @brief Checks if class has a public member function <code>{mem.name}</code>
 */
template<>
struct ClassMemberExists<{ns_name}::{decl.name}, decltype("{mem.name}"_method)> : std::true_type {{}};
""")
        elif isinstance(mem, declarations.class_declaration.class_t):
            expand_class(out, f"{ns_name}::{decl.name}", mem)

        elif isinstance(mem, declarations.enumeration_t):
            expand_enum(out, f"{ns_name}::{decl.name}", mem)


def generate_meta(args, xml_generator_config):
    output = args.output_meta
    base, ext = os.path.splitext(os.path.split(output.upper())[-1])
    include_gaurd = f"__ABOUT_AUTO_GENERATED__{base}"
    include_gaurd = include_gaurd.replace("-", "_")
    include_gaurd = include_gaurd.replace(".", "_")

    with open_output_handle(output) as out:
        out.write(START_OF_FILE.format(gaurd=include_gaurd))
        for filename in args.inputs:
            out.write(f"#include \"{filename}\"\n");

            out.write("""
namespace about
{
namespace detail
{
""")
        for filename in args.inputs:
            # Parse the code
            decls = xml_parser.parse([filename], xml_generator_config)

            # Get access to the global namespace
            global_ns = declarations.get_global_namespace(decls)

            for n in global_ns.declarations:
                if isinstance(n, declarations.namespace_t):
                    inner_ns = global_ns.namespace(n.name)
                    for n in inner_ns.declarations:
                        if isinstance(n, declarations.class_t):
                            expand_class(out, inner_ns.name, n)
                        elif isinstance(n, declarations.enumeration_t):
                            expand_enum(out, inner_ns.name, n)
        out.write("""
} // namespace detail
} // namespace about
""")
        out.write(END_OF_FILE.format(gaurd=include_gaurd))
