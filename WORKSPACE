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
    name = "spdlog",
    repository = "gabime/spdlog",
    commit = "a4e6d8877c72f1fd33f1a50c7074a149782a7574",
    sha256 = "b50c6b93b1130bddf6242d68f9e0078d23ea34e949517da59ef306ba9fc9147b",
    build_file = "tools/spdlog.BUILD",
)
