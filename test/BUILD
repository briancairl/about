load("//:bazel/about.bzl", "cc_library_with_reflection")

cc_test(
  name="decl",
  srcs=["decl-test.cpp"],
  deps=["//:about", "@googletest//:gtest"],
  visibility=["//visibility:public"],
  timeout="short"
)

cc_library_with_reflection(
  name="test_classes_with_reflection",
  hdrs=["test_classes.hpp"],
  visibility=["//visibility:public"]
)

cc_test(
  name="meta",
  srcs=["meta-test.cpp"],
  deps=["//:about", "//:utility", "@googletest//:gtest", ":test_classes_with_reflection"],
  visibility=["//visibility:public"],
  timeout="short"
)

cc_test(
  name="enum-ostream",
  srcs=["enum-ostream-test.cpp"],
  deps=["//:about", "//:utility", "@googletest//:gtest", ":test_classes_with_reflection"],
  visibility=["//visibility:public"],
  timeout="short"
)

cc_test(
  name="filter",
  srcs=["filter-test.cpp"],
  deps=["//:utility", "@googletest//:gtest", ":test_classes_with_reflection"],
  visibility=["//visibility:public"],
  timeout="short"
)
