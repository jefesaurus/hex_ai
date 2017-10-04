#!/bin/bash -e
# Enters the docker container
DIR=$(dirname $(readlink -f "$0"))
$DIR/running_in_docker.sh
docker exec -it hex_ai_container $DIR/login.sh $@
