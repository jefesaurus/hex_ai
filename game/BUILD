package(default_visibility = ["//visibility:public"])

cc_library(
    name = "piece_type",
    hdrs = ["piece_type.h"],
    deps = [],
)

cc_library(
    name = "zobrist",
    hdrs = ["zobrist.h"],
    deps = [
        ":piece_type",
        "//base",
    ],
)

cc_library(
    name = "hex_board",
    hdrs = ["hex_board.h"],
    deps = [
        ":piece_type",
        "//base",
        "//base:constexpr_ceil",
    ],
)

cc_test(
    name = "hex_board_test",
    srcs = ["hex_board_test.cc"],
    deps = [
        ":hex_board",
        "//base:gtest",
    ],
)

cc_library(
    name = "hex_state",
    hdrs = ["hex_state.h"],
    deps = [
        ":hex_board",
        ":zobrist",
    ],
)

cc_test(
    name = "hex_state_test",
    srcs = ["hex_state_test.cc"],
    deps = [
        ":hex_state",
        "//base:gtest",
    ],
)

cc_binary(
    name = "hex_state_speed_test",
    srcs = ["hex_state_speed_test.cc"],
    linkopts = [
        "-Wl,-no-as-needed",
        "-lprofiler",
    ],
    deps = [
        ":hex_state",
        "//base",
    ],
)
