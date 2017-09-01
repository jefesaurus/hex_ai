package(default_visibility = ["//visibility:public"])

alias(
    name = "buildifier",
    actual = "@com_github_bazelbuild_buildtools//buildifier",
)

filegroup(
    name = "empty",
    srcs = [],
)

cc_toolchain_suite(
    name = "CROSSTOOL",
    toolchains = {
        "local|compiler": ":cc-compiler-local",
    },
)

cc_toolchain(
    name = "cc-compiler-local",
    all_files = ":empty",
    compiler_files = ":empty",
    cpu = "local",
    dwp_files = ":empty",
    dynamic_runtime_libs = [":empty"],
    linker_files = ":empty",
    objcopy_files = ":empty",
    static_runtime_libs = [":empty"],
    strip_files = ":empty",
    supports_param_files = 1,
)
