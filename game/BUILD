package(default_visibility = ["//visibility:public"])

cc_library(
    name = "hex_state",
    srcs = ["hex_state.cc"],
    hdrs = ["hex_state.h"],
    deps = ["//base:logging"],
)

cc_binary(
    name = "hex_state_example",
    srcs = ["hex_state_example.cc"],
    deps = [
        ":hex_state",
        "//base:init",
    ],
)

cc_binary(
    name = "hex_state_speed_test",
    srcs = ["hex_state_speed_test.cc"],
    deps = [
        ":hex_state",
        "//base:init",
        "//base:wall_timer",
    ],
)
