#!/bin/bash
DIR="$(cd "$(dirname "$0")" && pwd -P)"
$DIR/docker/run_in_docker.sh bash
