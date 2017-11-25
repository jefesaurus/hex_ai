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
    deps = [":hex_env", ":memory"],
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

py_library(
    name = "memory",
    srcs = ["memory.py"],
)

py_test(
    name = "memory_test",
    srcs = ["memory_test.py"],
    deps = [":memory"],
)

py_library(
    name = "random_player",
    srcs = ["random_player.py"],
    deps = [":hex_env"],
)

py_library(
    name = "network_player",
    srcs = ["network_player.py"],
    deps = [":hex_env",
            ":learner",
            ":memory"],
)

py_binary(
    name = "run_loop",
    srcs = ["run_loop.py"],
    deps = [":random_player", ":network_player"],
)

py_binary(
    name = "test_loop",
    srcs = ["test_loop.py"],
    deps = [":random_player", ":network_player"],
)

py_test(
    name = "learner_test",
    srcs = ["learner_test.py"],
    deps = [":learner"],
)
