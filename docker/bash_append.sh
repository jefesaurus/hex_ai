#!/bin/bash
# This is sourced in the .bashrc within the container during "login"
export TERM=xterm
THIS_SCRIPT_DIR=$(dirname ${BASH_SOURCE[0]})
REPO_ROOT=$(dirname $THIS_SCRIPT_DIR)
BAZEL_OUTPUT_DIR=$REPO_ROOT/.bazel_output
alias bazel="bazel --output_base=$BAZEL_OUTPUT_DIR"
export REPO_ROOT
export PATH=$REPO_ROOT/tools/bin:$PATH
export PYTHONPATH=$REPO_ROOT:$PYTHONPATH
