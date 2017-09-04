package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "easyloggingpp",
    hdrs = ["src/easylogging++.h"],
    srcs = ["src/easylogging++.cc"],
    defines = ["ELPP_THREAD_SAFE"], 
    includes = ["src"],
    linkopts = ["-pthread"],
)
