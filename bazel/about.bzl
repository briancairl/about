
def cc_library_about(name, hdrs, **kwargs):
    gen_name = "__{name}_code_generation".format(name=name)
    out_files = ["{name}.hpp".format(name=name)]
    native.genrule(
        name = gen_name,
        outs = out_files,
        tools = ["//tools:about"],
        cmd = "$(location //tools:about) -i $(locations {hdrs}) -o $@".format(hdrs=' '.join(hdrs)),
        srcs = hdrs,
    )

    native.cc_library(
      name=name,
      hdrs=hdrs + out_files,
      **kwargs
    )
