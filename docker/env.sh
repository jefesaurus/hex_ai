#!/bin/bash -e
# Set up the environment from within docker and then run a command
DIR=$(dirname $(readlink -f "$0"))
REPO_ROOT=$(readlink -f $DIR/..)
. $REPO_ROOT/activate.sh
export TERM=xterm
$@
