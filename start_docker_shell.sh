#!/bin/bash
# Run a bash session in docker
DIR="$(cd "$(dirname "$0")" && pwd -P)"
$DIR/docker/run_in_docker.sh /bin/bash --login
