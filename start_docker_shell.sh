#!/bin/bash
DIR=$(dirname $(readlink -f "$0"))
$DIR/run_in_docker.sh bash
