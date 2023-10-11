#!/bin/bash

# Define the source files and output directory
PROTO_FILE=../app/arduino/proto.cpp
PROTO_DIR=../app/protocol
PROTO_WRAPPER_FILE=$PROTO_DIR/proto_wrapper.cpp


# Compile the Python bindings
g++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) $PROTO_FILE $PROTO_WRAPPER_FILE -o $PROTO_DIR/proto_module$(python3-config --extension-suffix)

# Print a success message
echo "Build completed successfully!"
