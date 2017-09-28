package(default_visibility = ["//visibility:public"])

proto_library(
    name = "proto_test_proto",
    srcs = ["proto_test.proto"],
)

cc_proto_library(
    name = "proto_test_cc_proto",
    deps = ["proto_test_proto"],
)

cc_test(
    name = "proto_test",
    srcs = ["proto_test.cc"],
    deps = ["proto_test_cc_proto", "//base:gtest"],
)
