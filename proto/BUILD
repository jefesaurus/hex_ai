package(default_visibility = ["//visibility:public"])

load("@org_pubref_rules_protobuf//cpp:rules.bzl", "cc_proto_library")
load("@org_pubref_rules_protobuf//python:rules.bzl", "py_proto_library")

cc_proto_library(
    name = "hex_state_cc_proto",
    protos = ["hex_state.proto"],
    with_grpc = True,
)

py_proto_library(
    name = "hex_state_py_proto",
    protos = ["hex_state.proto"],
    with_grpc = True,
)

cc_test(
    name = "hex_state_test",
    srcs = ["hex_state_test.cc"],
    deps = [
        ":hex_state_cc_proto",
        "//base:gtest",
    ],
)

cc_library(
    name = "proto_file",
    srcs = ["proto_file.h"],
    linkopts = ["-lstdc++fs"],
    deps = [
        "//base:logging",
        "@com_google_protobuf//:protobuf",
    ],
)

cc_proto_library(
    name = "test_cc_proto",
    protos = ["test.proto"],
    with_grpc = True,
)

py_proto_library(
    name = "test_py_proto",
    protos = ["test.proto"],
    with_grpc = True,
)

filegroup(
    name = "testdata",
    srcs = glob(["testdata/*"]),
)

cc_test(
    name = "proto_file_test",
    srcs = ["proto_file_test.cc"],
    data = [":testdata"],
    deps = [
        ":proto_file",
        ":test_cc_proto",
        "//base:gtest",
    ],
)

py_library(
    name = "proto_file_py",
    srcs = ["proto_file.py"],
    deps = [
        "@com_google_protobuf//:protobuf_python",
    ],
)

py_test(
    name = "proto_file_py_test",
    size = "small",
    srcs = ["proto_file_py_test.py"],
    deps = [
        ":proto_file_py",
        ":test_py_proto",
    ],
)
