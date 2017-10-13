#!/bin/bash -e
# Builds the docker image then attemps to upload it.
DIR="$(cd "$(dirname "$0")" && pwd -P)"
IMAGE_TAG=glalonde/hex_ai
echo "Image: $IMAGE_TAG"
cd $DIR
docker build --tag=$IMAGE_TAG .
docker login --username=glalonde
docker push $IMAGE_TAG \
    || echo "WARNING: Failed to upload docker image to cloud storage"
