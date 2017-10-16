#!/bin/bash
REPO_ROOT=$1
# Run this at the end
COMMAND=${@:2}
SOURCE_SCRIPT=$REPO_ROOT/docker/bash_append.sh
ALIAS_COMMAND=$(printf "source %s\n" $SOURCE_SCRIPT)
echo $ALIAS_COMMAND >> ~/.bashrc
cd $REPO_ROOT
$COMMAND
