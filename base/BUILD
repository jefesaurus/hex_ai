package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "init_example",
    srcs = ["init_example.cc"],
    deps = [":init"],
)

cc_library(
    name = "base",
    hdrs = [
        "never_destroyed.h",
    ],
)

cc_library(
    name = "init",
    hdrs = ["init.h"],
    deps = [":logging"],
)

cc_library(
    name = "logging",
    srcs = ["logging.cc"],
    hdrs = ["logging.h"],
    linkopts = [
        "-lgflags",
    ],
    deps = [
        ":base",
        "@spdlog//:spdlog",
    ],
)

cc_library(
    name = "time",
    hdrs = ["time.h"],
    deps = [":logging"],
)

cc_library(
    name = "wall_timer",
    hdrs = ["wall_timer.h"],
    deps = [
        ":logging",
        ":time",
    ],
)

cc_library(
    name = "file",
    srcs = ["file.cc"],
    hdrs = ["file.h"],
    deps = [
        ":logging",
    ],
)
