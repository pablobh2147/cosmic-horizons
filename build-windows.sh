#!/bin/bash

# Cross-compilation script for building Cosmic Horizons for Windows on Linux
# Requires: mingw-w64 toolchain

set -e

echo "=== Cosmic Horizons - Windows Cross-Compilation ==="
echo ""

# Check if MinGW-w64 is installed
if ! command -v x86_64-w64-mingw32-gcc &> /dev/null; then
    echo "Error: MinGW-w64 is not installed."
    echo "Please install it using:"
    echo "  Ubuntu/Debian: sudo apt install mingw-w64"
    echo "  Fedora: sudo dnf install mingw64-gcc mingw64-gcc-c++"
    echo "  Arch: sudo pacman -S mingw-w64-gcc"
    exit 1
fi

# Build directory
BUILD_DIR="build-windows"
TOOLCHAIN_FILE="cmake/toolchain-mingw-w64.cmake"

echo "Toolchain: MinGW-w64"
echo "Build directory: $BUILD_DIR"
echo ""

# Configure
echo "Configuring CMake..."
cmake -B "$BUILD_DIR" \
    -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
    -DCMAKE_BUILD_TYPE=Release \
    -DPLATFORM=Desktop

echo ""
echo "Building..."
cmake --build "$BUILD_DIR" --config Release -j$(nproc)

echo ""
echo "=== Build Complete ==="
echo "Windows executable: $BUILD_DIR/CosmicHorizons.exe"
echo ""
echo "To package for distribution, copy the following to a folder:"
echo "  - $BUILD_DIR/CosmicHorizons.exe"
echo "  - assets/ (entire directory)"
echo ""
