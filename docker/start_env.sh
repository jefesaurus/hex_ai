#!/bin/bash -e
set -o xtrace

# Kills the existing container and starts a new one

DIR="$(cd "$(dirname "$0")" && pwd -P)"
$DIR/inside_docker.sh
REPO_ROOT="$(cd "$(dirname "$DIR/..")" && pwd -P)"
CONTAINER_BASE=$(basename $REPO_ROOT)
CONTAINER_ID=hex_ai_container
CONTAINER_TAG=glalonde/hex_ai

docker rm -f $CONTAINER_ID || true
docker pull $CONTAINER_TAG || $REPO_ROOT/docker/build.sh

docker run \
    -d \
    --name=$CONTAINER_ID \
    --net=host \
    --pid=host \
    -e TARGET_USER=$USER \
    -e REPO_ROOT=$REPO_ROOT \
    -v $REPO_ROOT:$REPO_ROOT \
    -v $REPO_ROOT/docker/container_init.sh:/root/container_init.sh \
    --privileged \
    -h devenv \
    $CONTAINER_TAG \
    $REPO_ROOT/docker/wait_forever.sh

echo Waiting for docker container $CONTAINER_ID to be in the running state.
until [ "$(docker inspect -f {{.State.Running}} $CONTAINER_ID)" == "true" ]; do
  sleep 0.1;
done

# container_init.sh creates this file when it starts, see wait_forever.sh
ENTRY_FILE=/tmp/hex_ai_container
echo Waiting for docker container $CONTAINER_ID to execute container_init.sh
until [ "$(docker exec $CONTAINER_ID ls $ENTRY_FILE 2> /dev/null)" == "$ENTRY_FILE" ]; do
  sleep 0.1;
done
echo done
