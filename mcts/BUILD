package(default_visibility = ["//visibility:public"])

cc_library(
    name = "mcts",
    hdrs = ["mcts.h"],
    deps = [
        "//game:hex_state",
    ],
)

cc_binary(
    name = "tree_test",
    srcs = ["tree_test.cc"],
    deps = [
        ":mcts",
    ],
)

cc_binary(
    name = "iter_test",
    srcs = ["iter_test.cc"],
    deps = [
        ":mcts",
    ],
)

cc_binary(
    name = "tree_speed_test",
    srcs = ["tree_speed_test.cc"],
    linkopts = [
        "-Wl,-no-as-needed",
        "-lprofiler",
    ],
    deps = [
        ":mcts",
    ],
)

cc_test(
    name = "junction_test",
    srcs = ["junction_test.cc"],
    deps = [
        "//base:gtest",
        "@junction_turf_bazel//:junction",
    ],
)
