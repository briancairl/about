load("//:bazel/about.bzl", "cc_library_with_reflection")

cc_test(
  name="decl",
  srcs=["decl-test.cpp"],
  deps=["//:decl", "@googletest//:gtest"],
  visibility=["//visibility:public"],
  timeout="short"
)

cc_library_with_reflection(
  name="gen-test-about",
  hdrs=["gen-test.hpp"],
  visibility=["//visibility:public"]
)

cc_test(
  name="gen",
  srcs=["gen-test.cpp"],
  deps=["//:decl", "//:utility", "@googletest//:gtest", ":gen-test-about"],
  visibility=["//visibility:public"],
  timeout="short"
)


cc_test(
  name="filter",
  srcs=["filter-test.cpp"],
  deps=["//:utility", "@googletest//:gtest", ":gen-test-about"],
  visibility=["//visibility:public"],
  timeout="short"
)
