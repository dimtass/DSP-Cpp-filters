#!/bin/bash -e
echo "Building the project in Linux environment"

# Clean build directories
: ${CLEANBUILD:=true}
# Enable build unit-tests
: ${BUILD_UNIT_TESTS:=ON}

# Set default arch to stm32
BUILD_DIR=build
# Current working directory
WORKING_DIR=$(pwd)
# Compile objects in parallel, the -jN flag in make
PARALLEL=$(nproc)

CMAKE_FLAGS="${CMAKE_FLAGS} \
            -DBUILD_UNIT_TESTS=${BUILD_UNIT_TESTS} \
            "

if [ "${CLEANBUILD}" == "true" ]; then
    echo "- removing build directory: ${BUILD_DIR}"
    rm -rf ${BUILD_DIR}
fi

mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

# setup cmake
cmake .. ${CMAKE_FLAGS}

# build
make -j${PARALLEL} --no-print-directory
make check