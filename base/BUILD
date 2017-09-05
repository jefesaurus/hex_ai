package(default_visibility = ["//visibility:public"])

cc_library(
    name = "base",
    linkopts = ["-lm"],
    deps = [
        ":init",
        ":logging",
        ":time",
        ":wall_timer",
    ],
)

cc_library(
    name = "init",
    hdrs = ["init.h"],
    deps = [":logging"],
)

cc_library(
    name = "logging",
    hdrs = ["logging.h"],
    linkopts = [
        "-lgflags",
    ],
    deps = [
        "@easyloggingpp//:easyloggingpp",
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

cc_binary(
    name = "logging_example",
    srcs = ["logging_example.cc"],
    deps = [":logging"],
)

cc_binary(
    name = "init_example",
    srcs = ["init_example.cc"],
    deps = [":init"],
)
