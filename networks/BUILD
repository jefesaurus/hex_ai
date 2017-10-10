package(default_visibility = ["//visibility:public"])

cc_binary(
    name = "tiny_dnn_example",
    srcs = ["tiny_dnn_example.cc"],
    data = [
        "@tiny_dnn//:example_data",
    ],
    deps = [
        "//base:init",
        "@tiny_dnn//:tiny_dnn",
    ],
)
