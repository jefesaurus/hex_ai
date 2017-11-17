package(default_visibility = ["//visibility:public"])

py_test(
    name = "tensorflow_test",
    srcs = ["tensorflow_test.py"],
)

py_library(
    name = "hex_env",
    srcs = ["hex_env.py"],
)

py_library(
    name = "learner",
    srcs = ["learner.py"],
    deps = [":hex_env"],
)


py_binary(
    name = "loop",
    srcs = ["loop.py"],
    deps = [":learner"],
)

py_test(
    name = "hex_env_test",
    srcs = ["hex_env_test.py"],
    deps = [":hex_env"],
)
