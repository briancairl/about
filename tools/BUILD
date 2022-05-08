py_library(
    name = "impl",
    srcs = glob(["about/*.py"]),
    visibility = ["//visibility:private"],
    deps = [],
)

py_binary(
    name = "about",
    srcs = ["about.py"],
    visibility = ["//visibility:public"],
    deps = [":impl"],
)
