#!/bin/python

# Standard Library
import os
from typing import (Dict, List, Optional)

# PyGCCXML
from pygccxml import declarations

# About
from impl.common import open_output_handle

START_OF_FILE = """
/**
 * THIS CODE WAS AUTO-GENERATED
 */
#ifndef {gaurd}__ENUM_OSTREAM_HPP
#define {gaurd}__ENUM_OSTREAM_HPP


// C++ Standard Library
#include <ostream>

// About
#include <about/about.hpp>

// USER LIBRARIES

"""

END_OF_FILE = """
#endif // {gaurd}__ENUM_OSTREAM_HPP
"""

def expand_class(out, ns_name:str, decl):
    for mem in decl.public_members:
        if isinstance(mem, declarations.class_declaration.class_t):
            expand_class(out, f"{ns_name}::{decl.name}", mem)

        elif isinstance(mem, declarations.enumeration_t):
            expand_enum(out, f"{ns_name}::{decl.name}", mem)


def expand_enum(out, ns_name:str, decl):
    fully_qualified_enum_name = f"{ns_name}::{decl.name}"
    out.write(f"""
inline std::ostream& operator<<(std::ostream& os, const {fully_qualified_enum_name} e)
{{
    switch (e)
    {{
""")

    for label, value in decl.values:
        out.write(f"""
        case {fully_qualified_enum_name}::{label}:
        {{
            return os << \"{decl.name}::{label}\";
        }}
""")

    out.write(f"""
        default:
        {{
            break;
        }}
    }}
    return os << "INVALID <<{decl.name}>>";
}}
""")


def generate_enum_ostream(args, decls):
    if not args.output_enum_ostream:
        return
    output = args.output_enum_ostream
    base, ext = os.path.splitext(os.path.split(output.upper())[-1])
    include_gaurd = f"__ABOUT_AUTO_GENERATED__{base}"
    include_gaurd = include_gaurd.replace("-", "_")
    include_gaurd = include_gaurd.replace(".", "_")

    with open_output_handle(args.output_enum_ostream) as out:
        out.write(START_OF_FILE.format(gaurd=include_gaurd))
        for filename in args.inputs:
            out.write(f"#include \"{filename}\"\n");

            out.write("""
namespace about
{
""")
        # Get access to the global namespace
        global_ns = declarations.get_global_namespace(decls)

        for n in global_ns.declarations:
            if isinstance(n, declarations.namespace_t):
                inner_ns = global_ns.namespace(n.name)
                for n in inner_ns.declarations:
                    if isinstance(n, declarations.enumeration_t):
                        expand_enum(out, inner_ns.name, n)
                    elif isinstance(n, declarations.class_t):
                        expand_class(out, inner_ns.name, n)
        out.write("""
} // namespace about
""")
        out.write(END_OF_FILE.format(gaurd=include_gaurd))
