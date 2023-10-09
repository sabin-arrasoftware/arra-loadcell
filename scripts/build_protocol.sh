#!/bin/bash

# Define the source files and output directory
PROTO_DIR="../app/protocol"

# Compile the Python bindings
g++ -O3 -Wall -shared -std=c++11 -fPIC $(python3 -m pybind11 --includes) $PROTO_DIR/proto.cpp $PROTO_DIR/proto_wrapper.cpp -o $PROTO_DIR/proto_module$(python3-config --extension-suffix)

# Print a success message
echo "Build completed successfully!"
