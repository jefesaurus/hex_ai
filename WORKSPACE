load("//tools:github.bzl", "github_archive")

# Necessary for buildifier.
github_archive(
    name = "io_bazel_rules_go",
    repository = "bazelbuild/rules_go",
    commit = "0.4.4",
    sha256 = "afec53d875013de6cebe0e51943345c587b41263fdff36df5ff651fbf03c1c08",  # noqa
)

# Necessary for buildifier.
load("@io_bazel_rules_go//go:def.bzl", "go_repositories", "new_go_repository")

# Necessary for buildifier.
go_repositories()

# Necessary for buildifier.
new_go_repository(
    name = "org_golang_x_tools",
    commit = "3d92dd60033c312e3ae7cac319c792271cf67e37",
    importpath = "golang.org/x/tools",
)

github_archive(
    name = "com_github_bazelbuild_buildtools",
    repository = "bazelbuild/buildtools",
    commit = "7ce605fb1585076ed681e37d82d0ef529244b23a",
    sha256 = "c6210992d328212a7752a2c888a15f5c597dbf31f03ac0d59457ceff2928a30b",
)

github_archive(
    name = "easyloggingpp",
    repository = "muflihun/easyloggingpp",
    commit = "468597048ed3797e9556237f8a3c014944ae126b",
    sha256 = "3ac7ea8e56c3ed9701c564d704b63fe7379473fe85001a85ea5cc8bd95f02886",
    build_file = "tools/easyloggingpp.BUILD",
)
