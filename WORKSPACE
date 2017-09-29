load("//tools:github.bzl", "github_archive")

# Necessary for buildifier.
github_archive(
    name = "io_bazel_rules_go",
    repository = "bazelbuild/rules_go",
    commit = "0.5.5",
    sha256 = "fe5fd71624db32f01ce3445ff54dcbc038666847f688695fd3a13859e222ae7c",
)

# Necessary for buildifier.
load("@io_bazel_rules_go//go:def.bzl", "go_repositories")

# Necessary for buildifier.
go_repositories()

github_archive(
    name = "easyloggingpp",
    repository = "muflihun/easyloggingpp",
    commit = "468597048ed3797e9556237f8a3c014944ae126b",
    sha256 = "b63779b62fd588cfebf3f3a65f2bc5db86527a5cd0d09b30ce4bf64063098f85",
    build_file = "tools/easyloggingpp.BUILD",
)

github_archive(
    name = "com_google_googletest",
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

github_archive(
    name = "junction_turf_bazel",
    repository = "jefesaurus/junction_turf_bazel",
    commit = "64ac17867f17dedc990a0d3518fe6edc2daa60cd",
    sha256 = "8d4589bfd2b2cc799cbd841d8380f6c44b853c9f1882807f9e03459175a7406d",
)

github_archive(
  name = "org_pubref_rules_protobuf",
  repository = "pubref/rules_protobuf",
  commit = "v0.8.1",
  sha256 = "fb9852446b5ba688cd7178a60ff451623e4112d015c6adfe0e9a06c5d2dedc08",
)

load("@org_pubref_rules_protobuf//cpp:rules.bzl", "cpp_proto_repositories")
cpp_proto_repositories( excludes = ["com_google_googletest",])

load("@org_pubref_rules_protobuf//python:rules.bzl", "py_proto_repositories")
py_proto_repositories(omit_cpp_repositories = True, excludes = ["com_google_googletest",])


# Bazel python rules
github_archive(
    name = "io_bazel_rules_python",
    repository = "bazelbuild/rules_python",
    commit = "fa77c9c1118380e066c88b955c90fb3c7353429e",
    sha256 = "ea23e1b12287f55b3948fd5f66bc5a49ab40e9962107853d7618cfd535823390",
)

load("@io_bazel_rules_python//python:pip.bzl", "pip_repositories", "pip_import")

pip_repositories()

pip_import(
   name = "pip_grpcio",
   requirements = "@org_pubref_rules_protobuf//python:requirements.txt",
)

load("@pip_grpcio//:requirements.bzl", pip_grpcio_install = "pip_install")

pip_grpcio_install()
