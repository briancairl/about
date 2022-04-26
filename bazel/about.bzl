
def about(name, srcs, strip_include_prefix=None, include_prefix=None):
    gen_name = "__{name}_code_generation".format(name=name)
    out_files = ["{s}.about".format(s=s) for s in srcs]
    native.genrule(
        name = gen_name,
        outs = out_files,
        tools = ["//tools:about"],
        cmd = "$(location //tools:about) -i $(locations {srcs}) -o $@".format(srcs=' '.join(srcs)),
        srcs = srcs,
    )

    native.cc_library(
      name=name,
      hdrs=out_files,
      include_prefix=include_prefix,
      strip_include_prefix=strip_include_prefix,
      visibility = ["//visibility:public"],
    )
