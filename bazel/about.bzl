
def cc_library_about(name, hdrs, deps=[], **kwargs):
    gen_name = "__{name}_code_generation".format(name=name)

    out_meta_header = "{name}.hpp".format(name=name)
    out_enum_ostream_header = "{name}.enum.hpp".format(name=name)
    out_files = [out_meta_header, out_enum_ostream_header]

    native.genrule(
        name = gen_name,
        outs = out_files,
        tools = ["//tools:about"],
        cmd = "$(location //tools:about) -i $(locations {input_hdrs}) -o $(location {out_meta_hdr}) -oe $(location {out_enum_ostream_hdr})".format(
            input_hdrs=' '.join(hdrs),
            out_meta_hdr = out_meta_header,
            out_enum_ostream_hdr = out_enum_ostream_header,
        ),
        srcs = hdrs,
    )

    native.cc_library(
      name=name,
      hdrs=hdrs + out_files,
      deps=["//:decl"] + deps,
      **kwargs
    )
