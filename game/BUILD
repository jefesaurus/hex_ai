package(default_visibility = ["//visibility:public"])

cc_library(
    name = "piece_type",
    hdrs = ["piece_type.h"],
    deps = [],
)

cc_library(
    name = "hex_state",
    hdrs = ["hex_state.h"],
    deps = [
        ":piece_type",
        "//base",
    ],
)

cc_binary(
    name = "hex_state_example",
    srcs = ["hex_state_example.cc"],
    deps = [
        ":hex_state",
    ],
)

cc_binary(
    name = "hex_state_speed_test",
    srcs = ["hex_state_speed_test.cc"],
    linkopts = [
        "-Wl,-no-as-needed",
        "-lprofiler",
    ],
    #copts = ["-pg"],
    deps = [
        ":hex_state",
        "//base",
    ],
)
