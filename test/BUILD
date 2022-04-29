load("//:bazel/about.bzl", "about")

cc_test(
  name="common",
  srcs=["common-test.cpp"],
  deps=["//:common", "@googletest//:gtest"],
  visibility=["//visibility:public"],
  timeout="short"
)

about(name="gen-test-about", srcs=["gen-test.hpp"], strip_include_prefix="/test")

cc_library(
  name="gen-test",
  hdrs=["gen-test.hpp"],
  strip_include_prefix="/test",
  include_prefix="test",
  visibility=["//visibility:public"],
)

cc_test(
  name="gen",
  srcs=["gen-test.cpp"],
  deps=["//:common", "//:utility", "@googletest//:gtest", ":gen-test", ":gen-test-about"],
  visibility=["//visibility:public"],
  timeout="short"
)
