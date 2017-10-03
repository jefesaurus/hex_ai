package(default_visibility = ["//visibility:public"])

load("@org_pubref_rules_protobuf//cpp:rules.bzl", "cc_proto_library")
load("@org_pubref_rules_protobuf//python:rules.bzl", "py_proto_library")

filegroup(
    name = "protos",
    srcs = ["game_protocol.proto"],
)

cc_proto_library(
    name = "game_protocol_cc_proto",
    protos = [":protos"],
    verbose = 0,
    with_grpc = True,
)

py_proto_library(
    name = "game_protocol_py_proto",
    protos = [
        ":protos",
    ],
    with_grpc = True,
)

# Game server library
cc_library(
    name = "hexecutive",
    srcs = ["hexecutive.h"],
    deps = [":game_protocol_cc_proto"],
)

cc_binary(
    name = "hexecutive_main",
    srcs = ["hexecutive_main.cc"],
    deps = [
        ":hexecutive",
        "//base:logging",
    ],
)
