package(default_visibility = ["//visibility:public"])

cc_library(
    name = "cell_state",
    hdrs = ["cell_state.h"],
    deps = [],
)

cc_library(
    name = "hex_state",
    srcs = ["hex_state.cc"],
    hdrs = ["hex_state.h"],
    deps = [
        ":cell_state",
        "//base:logging",
    ],
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
