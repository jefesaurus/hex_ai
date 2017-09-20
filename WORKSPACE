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
    sha256 = "b63779b62fd588cfebf3f3a65f2bc5db86527a5cd0d09b30ce4bf64063098f85",
    build_file = "tools/easyloggingpp.BUILD",
)

github_archive(
    name = "googletest",
    repository = "google/googletest",
    commit = "deace2546ef4ff8f3e6349ec4f26d414753818c3",
    sha256 = "d4fae4f5206b66cc98eecb67c8e170b06256401fb69f1c41750fac1f227e4fda",
)

github_archive(
    name = "backwardcpp",
    repository = "bombela/backward-cpp",
    commit = "b4c35c3c876c42b3f4ea00a3bcbddf005aac5ad0",
    sha256 = "3cee96b881cdbeaf28a4c1f09a4e71392cdb9d095841007a71282dea5dc3c4f7",
    build_file = "tools/backwardcpp.BUILD",
)
