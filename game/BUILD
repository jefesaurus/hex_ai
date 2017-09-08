package(default_visibility = ["//visibility:public"])

cc_library(
    name = "piece_type",
    hdrs = ["piece_type.h"],
    deps = [],
)

cc_library(
    name = "hex_state",
    hdrs = ["hex_state.h"],
    deps = [
        ":piece_type",
        ":static_simd_bitset",
        "//base",
    ],
)

cc_binary(
    name = "hex_state_example",
    srcs = ["hex_state_example.cc"],
    deps = [
        ":hex_state",
    ],
)

cc_binary(
    name = "hex_state_speed_test",
    srcs = ["hex_state_speed_test.cc"],
    linkopts = [
        "-Wl,-no-as-needed",
        "-lprofiler",
    ],
    deps = [
        ":hex_state",
        "//base",
    ],
)

cc_library(
    name = "static_simd_bitset",
    hdrs = ["static_simd_bitset.h"],
    copts = [
        "-msse",
        "-msse2",
        "-msse3",
        "-mavx",
        "-mavx2",
    ],
    deps = [
        "//base",
    ],
)

cc_binary(
    name = "bitset_test",
    srcs = ["bitset_test.cc"],
    deps = [
        ":static_simd_bitset",
        "//base",
    ],
)
