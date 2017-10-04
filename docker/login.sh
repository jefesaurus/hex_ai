#!/bin/bash -e
# This logs in to the target user from within the container, then sets up the environment then runs the command
DIR=$(dirname $(readlink -f "$0"))
target_username="${TARGET_USER}"
exec sudo -Eiu ${target_username} $DIR/env.sh $@
