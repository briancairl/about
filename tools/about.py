#!/bin/python

# Standard Library
import argparse
import os
import sys
from typing import (Dict, List, Optional)

# PyGCCXML
from pygccxml import utils
from pygccxml import declarations
from pygccxml import parser as xml_parser


START_OF_FILE = """
/**
 * THIS CODE WAS AUTO-GENERATED
 */
#ifndef {gaurd}_HPP
#define {gaurd}_HPP


// C++ Standard Library
#include <type_traits>
#include <utility>

// About
#include <about/decl.hpp>

// USER LIBRARIES

"""

END_OF_FILE = """
}} // namespace about

#endif // {gaurd}_HPP
"""


def expand_class(out, ns_name:str, decl):
    _public_members_comma_sep = ', '.join([f"v.{v.name}" for v in decl.public_members if isinstance(v, declarations.variable_t)])
    out.write(f"""
template<>
struct Class<{ns_name}::{decl.name}>
{{
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
}};  // class Class<{ns_name}::{decl.name}>
""")

    for mem in decl.public_members:
        if isinstance(mem, declarations.variable_t):
            out.write(f"""
/**
 * @brief Checks if class has a public member variable <code>{mem.name}</code>
 */
template<>
struct ClassHas<{ns_name}::{decl.name}, decltype("{mem.name}"_member)> : std::true_type {{}};
""")
        elif isinstance(mem, declarations.member_function_t) or isinstance(mem, declarations.member_operator_t):
            out.write(f"""
/**
 * @brief Checks if class has a public member function <code>{mem.name}</code>
 */
template<>
struct ClassHas<{ns_name}::{decl.name}, decltype("{mem.name}"_method)> : std::true_type {{}};
""")
        elif isinstance(mem, declarations.class_declaration.class_t):
            expand_class(out, f"{ns_name}::{decl.name}", mem)

def open_output_handle(filename:Optional[str] = None):
    if filename:
        return open(filename, 'w+')
    else:
        return sys.stdout

def generate_common(args):
    output = args.output
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
        out.write(END_OF_FILE.format(gaurd=include_gaurd))


def expand_enum(out, ns_name:str, decl):
    full_enum_name = f"{ns_name}::{decl.name}"
    out.write(f"""
inline std::ostream& operator<<(std::ostream& os, const {full_enum_name} e)
{{
    switch (e)
    {{
""")

    for label, value in decl.values:
        out.write(f"""
        case {full_enum_name}::{label}:
            return os << \"{full_enum_name}::{label}\";
""")


    out.write(f"""
        default:
            break;
    }}
    return os << "INVALID <<{full_enum_name}>>";
}}
""")


def generate_enum_utilities(args):
    if not args.output_enum:
        return
    output = args.output_enum
    base, ext = os.path.splitext(os.path.split(output.upper())[-1])
    include_gaurd = f"__ABOUT_AUTO_GENERATED__{base}"
    include_gaurd = include_gaurd.replace("-", "_")
    include_gaurd = include_gaurd.replace(".", "_")

    with open_output_handle(args.output_enum) as out:
        out.write(START_OF_FILE.format(gaurd=include_gaurd))
        for filename in args.inputs:
            out.write(f"#include \"{filename}\"\n");

            out.write("""
namespace about
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
                        if isinstance(n, declarations.enumeration_t):
                            expand_enum(out, inner_ns.name, n)
                        # elif isinstance(n, declarations.class_t):
                        #     expand_class_enum(out, inner_ns.name, n)
        out.write(END_OF_FILE.format(gaurd=include_gaurd))


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--inputs", nargs="*", required=True, help="Input file paths", default=None)
    parser.add_argument("-o", "--output", type=str, help="Output file path", default=None)
    parser.add_argument("-oe", "--output-enum", type=str, help="Output file path for enum utilities", default=None)
    args = parser.parse_args()

    # Find the location of the xml generator (castxml or gccxml)
    generator_path, generator_name = utils.find_xml_generator()

    # Configure the xml generator
    xml_generator_config = xml_parser.xml_generator_configuration_t(
        xml_generator_path=generator_path,
        xml_generator=generator_name)

    generate_common(args=args)
    generate_enum_utilities(args=args)
