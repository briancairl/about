
def ast(name, hdrs, cxx):
    """
    Generates abstract syntax tree a structured text (JSON)
    """
    ast_out = "{}.json".format(name)
    ast_cmd = "clang++-9 -Xclang -ast-dump=json -fsyntax-only $(locations {}) -std={} > $(location {})".format(' '.join(hdrs), cxx, ast_out)
    native.genrule(
        name = name,
        outs = [ast_out],
        tools = ["//tools:about_clang_ast"],
        cmd = ast_cmd,
        srcs = hdrs,
    )
    return ast_out


def reflection(name, ast):
    """
    Generates reflection headers from input header files, hdrs
    """
    refl_out = "{}.hpp".format(name)
    refl_cmd = "$(location //tools:about_clang_ast) $(location {}) $(location {})".format(refl_out, ast)

    print(refl_cmd)

    # Run the generation script
    native.genrule(
        name = name,
        outs = [refl_out],
        tools = ["//tools:about_clang_ast"],
        cmd = refl_cmd,
        srcs = [ast]
    )

def cc_library_with_reflection(name, hdrs, cxx, deps=[], **kwargs):
    """
    Generates reflection headers and creates a single library with input header files, hdrs, and generated header files
    """

    ast_path = ast(name="ast_{}".format(name), hdrs=hdrs, cxx=cxx)

    hdrs.append(reflection(name="refl_{}".format(name), ast=ast_path))

    native.cc_library(
        name=name,
        hdrs=hdrs,
        deps=["//:about"] + deps,
        **kwargs
    )
