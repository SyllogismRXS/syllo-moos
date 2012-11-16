#!/bin/bash

echo "Configuring..."
echo "cmake .."
mkdir ./build -p
pushd build >& /dev/null
cmake ..
echo "Building..."
echo "make $@"
make $@
popd >& /dev/null
