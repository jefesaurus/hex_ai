#!/bin/bash
running_in_docker() {
    awk -F/ '$2 == "docker"' /proc/self/cgroup | read
}
running_in_docker && echo "Already inside docker." && exit -1
exit 0
