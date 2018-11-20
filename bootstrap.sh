#!/bin/sh

SCRIPT=$(readlink -f "$0")
BASEDIR=$(dirname "$SCRIPT")
BUILD_DIR=_build

mkdir -p ${BUILD_DIR} && \
cd ${BUILD_DIR} && \
cmake -DCMAKE_BUILD_TYPE=Release ${BASEDIR} && \
make $@
