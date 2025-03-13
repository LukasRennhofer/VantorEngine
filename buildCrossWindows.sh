#!/bin/bash

echo "Cleaning old build..."
cd build
rm -rf windows
mkdir windows

cd windows

echo "Running CMake..."
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=../../toolchain-mingw.cmake ../..

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

cp $(x86_64-w64-mingw32-gcc -print-file-name=libgcc_s_seh-1.dll) ../windows
cp $(x86_64-w64-mingw32-gcc -print-file-name=libstdc++-6.dll) ../windows


echo "Build complete!"
read -p "Press any key to continue..."
