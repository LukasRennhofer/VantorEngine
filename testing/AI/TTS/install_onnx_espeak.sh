#!/bin/bash

# Set the installation directory
INSTALL_DIR="$HOME/onnx_espeak_installs"

# Create the installation directory
mkdir -p $INSTALL_DIR

# Install dependencies
echo "Installing dependencies..."
sudo apt-get update
sudo apt-get install -y build-essential libprotobuf-dev libprotoc-dev libncurses5-dev

# Install ONNX Runtime
echo "Downloading and installing ONNX Runtime..."
ONNX_RUNTIME_VERSION="1.21.0"
ONNX_RUNTIME_TAR="onnxruntime-linux-x64-${ONNX_RUNTIME_VERSION}.tar.gz"
ONNX_RUNTIME_URL="https://github.com/microsoft/onnxruntime/releases/download/v${ONNX_RUNTIME_VERSION}/${ONNX_RUNTIME_TAR}"

cd $INSTALL_DIR

# Download ONNX Runtime
wget $ONNX_RUNTIME_URL

# Extract ONNX Runtime
tar -xzvf $ONNX_RUNTIME_TAR

# Set environment variables for ONNX Runtime
echo "Setting up ONNX Runtime environment variables..."
echo "export ONNX_RUNTIME_PATH=$INSTALL_DIR/onnxruntime-linux-x64-${ONNX_RUNTIME_VERSION}" >> ~/.bashrc
echo "export LD_LIBRARY_PATH=\$LD_LIBRARY_PATH:\$ONNX_RUNTIME_PATH/lib" >> ~/.bashrc
echo "export PATH=\$PATH:\$ONNX_RUNTIME_PATH/bin" >> ~/.bashrc
source ~/.bashrc

# Install eSpeak NG
echo "Downloading and installing eSpeak NG..."
ESPEAK_NG_REPO="https://github.com/espeak-ng/espeak-ng.git"
ESPEAK_NG_DIR="$INSTALL_DIR/espeak-ng"

# Clone the eSpeak NG repository
git clone $ESPEAK_NG_REPO $ESPEAK_NG_DIR

# Build and install eSpeak NG
cd $ESPEAK_NG_DIR
make
sudo make install

# Set environment variables for eSpeak NG
echo "Setting up eSpeak NG environment variables..."
echo "export ESPEAK_NG_DIR=$INSTALL_DIR/espeak-ng" >> ~/.bashrc
source ~/.bashrc

# Installation complete
echo "ONNX Runtime and eSpeak NG installation is complete!"
echo "Please restart your terminal or run 'source ~/.bashrc' to apply the changes."
