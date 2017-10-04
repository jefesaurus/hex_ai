#!/bin/bash
DIR=$(dirname $(readlink -f "$0"))
$DIR/docker/run_in_docker.sh bash
