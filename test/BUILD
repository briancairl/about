load("//:bazel/about.bzl", "about")

cc_test(
  name="common",
  srcs=["common-test.cpp"],
  deps=["//:common", "@googletest//:gtest"],
  visibility=["//visibility:public"],
  timeout="short"
)

about(name="gen-test-code-generation", srcs=["gen-test.hpp"], strip_include_prefix="/test")

cc_library(
  name="gen-test",
  hdrs=["gen-test.hpp"],
  visibility=["//visibility:public"],
)

cc_test(
  name="gen",
  srcs=["gen-test.cpp"],
  deps=["//:common", "//:utility", "@googletest//:gtest", ":gen-test", ":gen-test-code-generation"],
  visibility=["//visibility:public"],
  timeout="short"
)
