#!/bin/bash

# Forces the command to be run inside the docker container (no-op if already in the docker container)
DIR="$(cd "$(dirname "$0")" && pwd -P)"
$DIR/inside_docker.sh
if [ $? -eq 0 ]; then
    echo $@
    $DIR/login.sh $@
else
    $@
fi
