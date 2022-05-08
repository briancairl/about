
def reflection(name, hdrs, enable_meta=True, enable_enum_ostream=True, __genrule_target_name=None):
    """
    Generates reflection headers from input header files, hdrs
    """
    if not (enable_meta or enable_enum_ostream):
        fail("At least one feature must be enabled! Otherwise, this rule has no affect.")

    __genrule_target_name = __genrule_target_name or name

    out_files = []
    cmd =  "$(location //tools:about) -i $(locations {})".format(' '.join(hdrs))

    # Use basic meta information generation feature
    if enable_meta:
        out_meta_header = "{name}.meta.hpp".format(name=name)
        out_files.append(out_meta_header)
        cmd += " -om $(location {})".format(out_meta_header)

    # Use enum-ostream overload generation feature
    if enable_enum_ostream:
        out_enum_ostream_header = "{name}.enum_ostream.hpp".format(name=name)
        out_files.append(out_enum_ostream_header)
        cmd += " -oe $(location {})".format(out_enum_ostream_header)

    # Run the generation script
    native.genrule(
        name = __genrule_target_name,
        outs = out_files,
        tools = ["//tools:about"],
        cmd = cmd,
        srcs = hdrs,
    )

    return out_files

def cc_library_with_reflection(name, hdrs, deps=[], enable_meta=True, enable_enum_ostream=True, **kwargs):
    """
    Generates reflection headers and creates a single library with input header files, hdrs, and generated header files
    """
    reflection_target_name = "__{name}_code_generation".format(name=name)
    native.cc_library(
        name=name,
        hdrs=hdrs + reflection(
            name=name,
            hdrs=hdrs,
            enable_meta=enable_meta,
            enable_enum_ostream=enable_enum_ostream,
            __genrule_target_name=reflection_target_name,
        ),
        deps=["//:about"] + deps,
        **kwargs
    )
