#!/bin/bash -e
# Builds the docker image then attemps to upload it.
DIR="$(cd "$(dirname "$0")" && pwd -P)"
cd $DIR

IMAGE_TAG=glalonde/hex_ai
DOCKER_FILE=Dockerfile

if [ $# -gt 0 ] && [ $1 = "gpu" ]
then
  echo "Building GPU image"
  IMAGE_TAG=glalonde/hex_ai_gpu
  DOCKER_FILE=Dockerfile.gpu
else
  echo "Building CPU image"
fi

docker build --tag=$IMAGE_TAG --file=$DOCKER_FILE .
docker login --username=glalonde
docker push $IMAGE_TAG \
    || echo "WARNING: Failed to upload docker image to cloud storage"
