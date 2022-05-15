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

cc_binary(
    name = "about_clang_ast",
    srcs = ["about_clang_ast.cpp"],
    visibility = ["//visibility:public"],
    deps = ["@picojson//:picojson",],
)