package(
    default_visibility = ["//visibility:public"],
)

cc_library(
    name = "easyloggingpp",
    srcs = ["src/easylogging++.cc"],
    hdrs = ["src/easylogging++.h"],
    defines = [
        "ELPP_FEATURE_CRASH_LOG",
        "ELPP_THREAD_SAFE",
        "ELPP_NO_DEFAULT_LOG_FILE",
        "ELPP_STL_LOGGING",
        "ELPP_LOG_STD_ARRAY",
        "ELPP_LOG_UNORDERED_MAP",
        "ELPP_LOG_UNORDERED_SET",
        "ELPP_DISABLE_DEFAULT_CRASH_HANDLING",
    ],
    includes = ["src"],
    linkopts = ["-pthread"],
)
