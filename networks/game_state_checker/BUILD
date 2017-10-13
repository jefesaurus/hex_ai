package(default_visibility = ["//visibility:public"])

load("@org_pubref_rules_protobuf//cpp:rules.bzl", "cc_proto_library")
load("@org_pubref_rules_protobuf//python:rules.bzl", "py_proto_library")

cc_proto_library(
    name = "data_cc_proto",
    proto_deps = ["//proto:hex_state_cc_proto"],
    protos = ["data.proto"],
)

py_proto_library(
    name = "data_py_proto",
    proto_deps = ["//proto:hex_state_py_proto"],
    protos = ["data.proto"],
)

cc_binary(
    name = "generate_data",
    srcs = ["generate_data.cc"],
    deps = [
        ":data_cc_proto",
        "//base:init",
        "//mcts",
        "//proto:proto_file",
    ],
)
