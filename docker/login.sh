#!/bin/bash -e
# This logs in to the target user from within the container, then sets up the environment then runs the command
DIR="$(cd "$(dirname "$0")" && pwd -P)"
target_username="${TARGET_USER}"
exec sudo -Eiu ${target_username} $DIR/env.sh $@
