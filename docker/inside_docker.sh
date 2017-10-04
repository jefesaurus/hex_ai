#!/bin/bash
inside_docker() {
    awk -F/ '$2 == "docker"' /proc/self/cgroup | read
}
inside_docker && echo "Already inside docker." && exit -1
exit 0
