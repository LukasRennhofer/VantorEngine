#!/bin/bash

VTRG_PATH="$(pwd)/vtrg.py"

# Destination for the symlink
TARGET_DIR="/usr/local/bin"

# Check if vtrg file exists
if [ ! -f "$VTRG_PATH" ]; then
    echo "❌ vtrg not found at $VTRG_PATH"
    exit 1
fi

# Make vtrg executable
chmod +x "$VTRG_PATH"

# Create a symlink to /usr/local/bin
echo "🔗 Linking $VTRG_PATH to $TARGET_DIR/vtrg..."
sudo ln -sf "$VTRG_PATH" "$TARGET_DIR/vtrg"

echo "✅ vtrg installed. Try running: vtrg --help"

# Usage:
# chmod +x setupEnvironment.sh
# ./setupEnvironment.sh
