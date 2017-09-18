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
