package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "backwardcpp",
    hdrs = ["backward.hpp"],
    defines = [
        "BACKWARD_HAS_BFD",
    ],
    includes = ["."],
    linkopts = [
        "-lbfd",
        "-ldl",
    ],
)
