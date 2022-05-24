
def cc_ast(name, hdrs, cxx):
    """
    Generates abstract syntax tree a structured text (JSON)
    """
    ast_out = "{}.json".format(name)
    ast_cmd = "clang++-9 -Xclang -ast-dump=json -fsyntax-only $(locations {}) -std={} > $(location {})".format(' '.join(hdrs), cxx, ast_out)
    native.genrule(
        name = "run_{}".format(name),
        outs = [ast_out],
        tools = ["//tools:about_clang_ast"],
        cmd = ast_cmd,
        srcs = hdrs,
    )
    native.filegroup(
        name = name,
        srcs = [ast_out]
    )


def cc_reflection(name, ast, output_filename=None):
    """
    Generates reflection headers from input header files, hdrs
    """
    refl_out = output_filename or "{}.hpp".format(name)
    refl_cmd = "$(location //tools:about_clang_ast) $(location {}) $(location {})".format(refl_out, ast)

    print(refl_cmd)

    # Run the generation script
    native.genrule(
        name = "run_{}".format(name),
        outs = [refl_out],
        tools = ["//tools:about_clang_ast"],
        cmd = refl_cmd,
        srcs = [ast]
    )
    native.filegroup(
        name = name,
        srcs = [refl_out]
    )


def cc_library_with_reflection(name, hdrs, cxx, deps=[], **kwargs):
    """
    Generates reflection headers and creates a single library with input header files, hdrs, and generated header files
    """

    cc_ast(name="ast_{}".format(name), hdrs=hdrs, cxx=cxx)

    cc_reflection(name="refl_{}".format(name), ast="ast_{}".format(name), output_filename="{}.hpp".format(name))

    native.cc_library(
        name=name,
        hdrs=hdrs + [":refl_{}".format(name)],
        deps=["//:about"] + deps,
        **kwargs
    )
