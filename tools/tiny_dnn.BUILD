package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "tiny_dnn",
    hdrs = glob([
        "tiny_dnn/**/*.h",
        "tiny_dnn/**/*.hpp",
        "cereal/**/*.h",
        "cereal/**/*.hpp",
    ]),
    defines = ["CNN_USE_TBB"],
    includes = ["."],
    linkopts = [
        "-lm",
        "-lpthread",
        "-ltbb",
    ],
)

filegroup(
    name = "example_data",
    srcs = glob(["data/**/*.*"]),
)
