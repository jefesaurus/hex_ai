#!/bin/bash
DIR="$(cd "$(dirname "$0")" && pwd -P)"
REPO_ROOT=$(dirname $DIR)
CONTAINER_TAG=glalonde/hex_ai
docker run \
    --rm \
    -it \
    -v $REPO_ROOT:$REPO_ROOT \
    -v $REPO_ROOT/docker/entrypoint.sh:/root/docker/entrypoint.sh \
    $CONTAINER_TAG \
    $REPO_ROOT \
    $@
