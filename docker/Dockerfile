FROM ubuntu:17.10

RUN apt-get -q update \
    && apt-get install --fix-missing -qy \
        apt-transport-https \
        apt-utils \
        curl \
        software-properties-common
RUN apt-get -q update \
    && apt-get install --fix-missing -qy \
        openjdk-8-jdk \
        openjdk-8-jre

RUN apt-add-repository "deb [arch=amd64] http://storage.googleapis.com/bazel-apt stable jdk1.8"
RUN curl -sS https://bazel.build/bazel-release.pub.gpg | apt-key add -

# Build tools
RUN apt-get -q update \
    && apt-get install --fix-missing -qy \
        bazel \
        clang-format \
        cmake \
        g++ \
        gcc \
        gdb \
        git \
        golang \
        sudo \
        valgrind \
        vim 

# Dev libs
RUN apt-get -q update \
    && apt-get install --fix-missing -qy \
        libgoogle-perftools-dev \
        libgflags-dev \
        libtbb-dev \
        binutils-dev

# Python
RUN apt-get -q update \
    && apt-get install --fix-missing -qy \
        pylint \
        python-dev  \
        python-numpy \
        python-pip \
        python-setuptools \
        python-scipy \
        python3-pip

RUN apt-get clean \
    && apt-get autoremove -qy \
    && rm -rf /var/lib/apt/lists/* \
    && rm -rf /tmp/*

ADD https://storage.googleapis.com/golang/go1.8.3.linux-amd64.tar.gz /tmp/golang.tar.gz
RUN cd /usr/local && \
    tar xvfz /tmp/golang.tar.gz && \
    cd /usr/local/bin && \
    ln -s /usr/local/go/bin/* . && \
    GOPATH=/usr go get github.com/bazelbuild/buildtools/buildifier && \
    chmod a+rx /usr/bin/buildifier

ENTRYPOINT ["/root/docker/entrypoint.sh"]  
