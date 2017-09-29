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
