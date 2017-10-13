#!/bin/bash -e
# Set up the environment from within docker and then run a command
DIR="$(cd "$(dirname "$0")" && pwd -P)"
. $DIR/../activate.sh
export TERM=xterm
$@
