#!/bin/python

# Standard Library
import argparse
import os
from typing import (Dict, List)

# PyGCCXML
from pygccxml import utils
from pygccxml import declarations
from pygccxml import parser as xml_parser


START_OF_FILE = """
#ifndef {gaurd}_HPP
#define {gaurd}_HPP

// C++ Standard Library
#include <type_traits>
#include <utility>

namespace about
{{
"""

END_OF_FILE = """
}} // namespace about

#endif // {gaurd}_HPP
"""


def expand_class(out, ns_name, decl):
    _public_members_comma_sep = ', '.join([f"v.{v.name}" for v in decl.public_members if isinstance(v, declarations.variable_t)])
    out.write(f"""
template<>
struct Class<{ns_name}::{decl.name}>
{{
""")

    out.write(f"""
/**
 * @brief Returns tuple of references to all public members
 * @note THIS CODE WAS AUTO-GENERATED
 */
static constexpr decltype(auto) public_members({ns_name}::{decl.name}& v)
{{
    return ::std::tie({_public_members_comma_sep});
}}

/**
 * @brief Returns tuple of const references to all public members
 * @note THIS CODE WAS AUTO-GENERATED
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
 * @note THIS CODE WAS AUTO-GENERATED
 */
template<>
struct ClassHas<{ns_name}::{decl.name}, decltype("{mem.name}"_member)> : std::true_type {{}};
""")
        elif isinstance(mem, declarations.member_function_t) or isinstance(mem, declarations.member_operator_t):
            out.write(f"""
/**
 * @brief Checks if class has a public member function <code>{mem.name}</code>
 * @note THIS CODE WAS AUTO-GENERATED
 */
template<>
struct ClassHas<{ns_name}::{decl.name}, decltype("{mem.name}"_method)> : std::true_type {{}};
""")
        elif isinstance(mem, declarations.class_declaration.class_t):
            expand_class(out, f"{ns_name}::{decl.name}", mem)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument("-i", "--inputs", nargs="*", required=True, help="Input file paths")
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("-o", "--output", type=str, help="Output file path")
    group.add_argument("-od", "--output-directory", type=str, help="Output directory")
    args = parser.parse_args()

    # Find the location of the xml generator (castxml or gccxml)
    generator_path, generator_name = utils.find_xml_generator()

    # Configure the xml generator
    xml_generator_config = xml_parser.xml_generator_configuration_t(
        xml_generator_path=generator_path,
        xml_generator=generator_name)

    if args.output:
        include_gaurd = f"__ABOUT_AUTO_GENERATED__{os.path.splitext(os.path.split(args.output.upper())[-1])[0]}"
        include_gaurd = include_gaurd.replace("-", "_")
        include_gaurd = include_gaurd.replace(".", "_")

        with open(args.output, 'w+') as out:
            out.write(START_OF_FILE.format(gaurd=include_gaurd))
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
    elif args.output_directory:
        for filename in args.inputs:
            input_filename_base = os.path.split(filename)[-1]
            output_filename_base, output_filename_ext = os.path.splitext(input_filename_base)
            output_path = os.path.join(args.output_directory, f"about_{input_filename_base}")

            include_gaurd = f"__ABOUT_AUTO_GENERATED__{output_filename_base.upper()}"
            include_gaurd = include_gaurd.replace("-", "_")
            include_gaurd = include_gaurd.replace(".", "_")

            with open(output_path, 'w+') as out:
                out.write(START_OF_FILE.format(gaurd=include_gaurd))

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
