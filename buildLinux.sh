#!/bin/bash

echo "Cleaning old build..."
cd build
rm -rf linux
mkdir linux

cd linux

echo "Running CMake..."
cmake -G "Unix Makefiles" ../..

echo "Building CHIFEngine..."
make -j$(nproc)

echo "Creating output directories..."
mkdir -p resources
mkdir -p shaders
mkdir -p libraries

echo "Copying Assets and Shaders..."
cp -r ../../src/resources resources
cp -r ../../src/shaders shaders
cp -r ../../src/libraries libraries

echo "Build complete!"
read -p "Press any key to continue..."
