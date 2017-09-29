package(default_visibility = ["//visibility:public"])

load("@org_pubref_rules_protobuf//cpp:rules.bzl", "cc_proto_library")
load("@org_pubref_rules_protobuf//python:rules.bzl", "py_proto_library")

filegroup(
    name = "protos",
    srcs = ["helloworld.proto"],
)

cc_proto_library(
    name = "helloworld_cc_proto",
    protos = [":protos"],
    verbose = 0,
    with_grpc = True,
)

py_proto_library(
    name = "helloworld_py_proto",
    protos = [
        ":protos",
    ],
    with_grpc = True,
)

# C++ rules
cc_test(
    name = "helloworld_grpc_test",
    size = "small",
    srcs = ["greeter_client_test.cc"],
    tags = ["exclusive"],
    deps = [
        ":clientlib",
        "//base:gtest",
    ],
)

cc_binary(
    name = "client",
    srcs = ["greeter_client_main.cc"],
    deps = [":clientlib"],
)

cc_binary(
    name = "server",
    srcs = ["greeter_server_main.cc"],
    deps = [":helloworld_cc_proto"],
)

cc_library(
    name = "clientlib",
    srcs = ["greeter_client.cc"],
    hdrs = ["greeter_client.h"],
    deps = [":helloworld_cc_proto"],
)

# Python rules
load("@pip_grpcio//:requirements.bzl", "requirement")

py_binary(
    name = "greeter_client",
    srcs = [
        "greeter_client.py",
    ],
    deps = [
        ":helloworld_py_proto",
        requirement("grpcio"),
    ],
)

py_binary(
    name = "greeter_server",
    srcs = [":grpc_server"],
)

py_library(
    name = "grpc_server",
    srcs = [
        "greeter_server.py",
    ],
    deps = [
        ":helloworld_py_proto",
        requirement("grpcio"),
    ],
)

py_test(
    name = "greeter_server_test",
    srcs = ["greeter_server_test.py"],
    deps = [":grpc_server"],
    size = "small",
)
