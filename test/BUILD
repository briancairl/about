load("//:bazel/about.bzl", "cc_library_about")

cc_test(
  name="common",
  srcs=["common-test.cpp"],
  deps=["//:common", "@googletest//:gtest"],
  visibility=["//visibility:public"],
  timeout="short"
)

cc_library_about(
  name="gen-test-about",
  hdrs=["gen-test.hpp"],
  visibility=["//visibility:public"]
)

cc_test(
  name="gen",
  srcs=["gen-test.cpp"],
  deps=["//:common", "//:utility", "@googletest//:gtest", ":gen-test-about"],
  visibility=["//visibility:public"],
  timeout="short"
)
