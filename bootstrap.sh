#!/bin/sh

BASEDIR=$(dirname "$0")
BUILD_DIR=_build

mkdir -p ${BUILD_DIR} && \
cd ${BUILD_DIR} && \
cmake -DCMAKE_BUILD_TYPE=Release ${BASEDIR} && \
make $@
