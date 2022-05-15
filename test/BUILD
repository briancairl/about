load("//:bazel/about.bzl", "cc_library_with_reflection")

cc_test(
  name="decl",
  srcs=["decl-test.cpp"],
  copts=["-Iexternal/googletest/googletest/include"],
  deps=["//:about", "@googletest//:gtest"],
  visibility=["//visibility:public"],
  timeout="short"
)

cc_library_with_reflection(
  name="test_classes_with_reflection",
  cxx="c++11",
  hdrs=["test_classes.hpp"],
  visibility=["//visibility:public"]
)

cc_test(
  name="meta",
  srcs=["meta-test.cpp"],
  copts=["-Iexternal/googletest/googletest/include"],
  deps=["//:about", "//:utility", "@googletest//:gtest", ":test_classes_with_reflection"],
  visibility=["//visibility:public"],
  timeout="short"
)

cc_test(
  name="enum-ostream",
  srcs=["enum-ostream-test.cpp"],
  copts=["-Iexternal/googletest/googletest/include"],
  deps=["//:about", "//:utility", "@googletest//:gtest", ":test_classes_with_reflection"],
  visibility=["//visibility:public"],
  timeout="short"
)

cc_test(
  name="filter",
  srcs=["filter-test.cpp"],
  copts=["-Iexternal/googletest/googletest/include"],
  deps=["//:utility", "@googletest//:gtest", ":test_classes_with_reflection"],
  visibility=["//visibility:public"],
  timeout="short"
)
