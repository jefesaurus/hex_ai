#!/bin/bash -e

if [[ $1 == "" ]]; then
  echo "Usage: $0 <bazel target name> <target args but not profilepath>"
  echo "eg.: $0 mcts/tree_speed_test --num_trials=100000"
  exit 1
fi
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
BAZEL_BIN_DIR=$SCRIPT_DIR/bazel-bin
TARGET_PATH=$1
TARGET_BIN=$BAZEL_BIN_DIR/$TARGET_PATH
PROF_FILE=$(mktemp /tmp/speed_prof.prof.XXXXXXXXX)
bazel build -c opt $TARGET_PATH
if [ ! -f $TARGET_BIN ]; then
    echo "File not found at $TARGET_BIN"
		exit 1
fi
$TARGET_BIN --profile_path=$PROF_FILE ${@:2:99}
CACHEGRIND_FILE=$(mktemp /tmp/speed_prof.cachegrind.XXXXXXXXX)
pprof --callgrind $TARGET_BIN $PROF_FILE > $CACHEGRIND_FILE
if [ ! -f $CACHEGRIND_FILE ]; then
    echo "Cachegrind file not found at $CACHEGRIND_FILE"
		exit 1
fi
echo "Cachegrind file written to: $CACHEGRIND_FILE"
kcachegrind $CACHEGRIND_FILE &
