
def reflection(name, hdrs, cxx, enable_meta=True, enable_enum_ostream=True, __genrule_target_name=None):
    """
    Generates reflection headers from input header files, hdrs
    """
    if not (enable_meta or enable_enum_ostream):
        fail("At least one feature must be enabled! Otherwise, this rule has no affect.")

    __genrule_target_name = __genrule_target_name or name

    output_files = []

    if enable_meta:
        output_files.append("{name}.meta.hpp".format(name=name))

    if enable_enum_ostream:
        output_files.append("{name}.enum_ostream.hpp".format(name=name))

    clang_ast_out = "{}_clang_ast_gen.json".format(name)
    clang_ast_gen = "clang++-9 -Xclang -ast-dump=json -fsyntax-only $(locations {}) -std={} > {}".format(' '.join(hdrs), cxx, clang_ast_out)
    cland_ast_consume = "$(location //tools:about_clang_ast) {} {}".format(clang_ast_out, ' '.join(["$(location {})".format(f) for f in output_files]))

    cmd =  "({}) && ({})".format(clang_ast_gen, cland_ast_consume)

    # Run the generation script
    native.genrule(
        name = __genrule_target_name,
        outs = output_files,
        tools = ["//tools:about_clang_ast"],
        cmd = cmd,
        srcs = hdrs,
    )

    return output_files

def cc_library_with_reflection(name, hdrs, cxx, deps=[], enable_meta=True, enable_enum_ostream=True, **kwargs):
    """
    Generates reflection headers and creates a single library with input header files, hdrs, and generated header files
    """
    reflection_target_name = "__{name}_code_generation".format(name=name)
    native.cc_library(
        name=name,
        hdrs=hdrs + reflection(
            name=name,
            hdrs=hdrs,
            cxx=cxx,
            enable_meta=enable_meta,
            enable_enum_ostream=enable_enum_ostream,
            __genrule_target_name=reflection_target_name,
        ),
        deps=["//:about"] + deps,
        **kwargs
    )
