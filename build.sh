#!/bin/sh

BUILD_DIR=build

[ -d ${BUILD_DIR} ] && rm -rf ${BUILD_DIR}

# Make build directory
mkdir ${BUILD_DIR} && cd ${BUILD_DIR}

# Configure and build
cmake .. -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=..
make -j$(nproc)
make install
cd ..
