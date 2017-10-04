#!/bin/bash

# Forces the command to be run inside the docker container (no-op if already in the docker container)

DIR=$(dirname $(readlink -f "$0"))
$DIR/running_in_docker.sh

if [ ! $? -eq 0 ]; then
    $DIR/login.sh $@
else
    CONTAINER_ID=hex_ai_container
    if ! docker ps | grep $CONTAINER_ID > /dev/null 2>&1 ; then
        $DIR/start_env.sh
    fi
    $DIR/open_shell.sh $@
fi
