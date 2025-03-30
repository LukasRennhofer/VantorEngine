#!/bin/bash

echo "Cleaning old build..."
cd build
rm -rf linux
mkdir linux

cd linux

echo "Running CMake..."
cmake -G "Unix Makefiles" ../../src/

echo "Building CHIFEngine..."
make 

echo "Creating output directories..."
mkdir -p resources
mkdir -p shaders
mkdir -p lib

echo "Copying Assets and Shaders..."
cp -r ../../src/resources resources
cp -r ../../src/shaders shaders
cp -r ../../src/lib lib

echo "Build complete!"
read -p "Press any key to continue..."
