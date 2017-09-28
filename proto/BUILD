package(default_visibility = ["//visibility:public"])

load("@com_google_protobuf//:protobuf.bzl", "py_proto_library")

proto_library(
    name = "proto_test_proto",
    srcs = ["proto_test.proto"],
)

cc_proto_library(
    name = "proto_test_cc_proto",
    deps = [":proto_test_proto"],
)

cc_test(
    name = "proto_test",
    srcs = ["proto_test.cc"],
    deps = [":proto_test_cc_proto", "//base:gtest"],
)

py_proto_library(
    name = "proto_test_py_proto",
    srcs = ["proto_test.proto"],
    protoc = "@com_google_protobuf//:protoc",
    default_runtime = "@com_google_protobuf//:protobuf_python",
)

py_binary(
    name = "python_test",
    srcs = ["python_test.py"],
    deps = [":proto_test_py_proto"],
)
