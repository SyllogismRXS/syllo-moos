#!/bin/bash

echo "Cleaning..."

rm -rf ./build
rm -rf ./lib
rm -rf ./bin
rm -rf ./include

if [ "$1" == all ]; then
    rm -rf /usr/local/include/syllo
    rm -rf /usr/local/lib/libsylloproto*
fi
