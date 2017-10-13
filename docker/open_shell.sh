#!/bin/bash -e
# Enters the docker container, login to target user and run a command
DIR="$(cd "$(dirname "$0")" && pwd -P)"
$DIR/inside_docker.sh
docker exec -it hex_ai_container $DIR/login.sh $@
