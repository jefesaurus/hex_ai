#!/bin/bash -e
# Enters the docker container, login to target user and run a command
DIR=$(dirname $(readlink -f "$0"))
$DIR/inside_docker.sh
docker exec -it hex_ai_container $DIR/login.sh $@
