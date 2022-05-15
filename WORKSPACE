load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# GTest
http_archive(
    name="googletest",
    url="https://github.com/google/googletest/archive/release-1.8.0.zip",
    sha256="f3ed3b58511efd272eb074a3a6d6fb79d7c2e6a0e374323d1e6bcbcc1ef141bf",
    strip_prefix="googletest-release-1.8.0",
    build_file="@//external:googletest.BUILD",
)

# Picojson
http_archive(
    name="picojson",
    url="https://github.com/kazuho/picojson/archive/v1.3.0.zip",
    sha256="58fff34589d93b0cef208a456788b9de6c7d04cfa9a7be576328ca7b48f10069",
    build_file="//external:picojson.BUILD",
    strip_prefix="picojson-1.3.0"
)
