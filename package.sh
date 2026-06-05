#!/bin/bash

set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_NAME="CosmicHorizons"
PACKAGE_DIR="${SCRIPT_DIR}/package"

LINUX_BUILD="${SCRIPT_DIR}/build"
WINDOWS_BUILD="${SCRIPT_DIR}/build-windows"
ASSETS_DIR="${SCRIPT_DIR}/assets"

echo "=== Cosmic Horizons - Packaging ==="
echo ""

# Check assets exist
if [ ! -d "$ASSETS_DIR" ]; then
    echo "Error: Assets directory not found at $ASSETS_DIR"
    exit 1
fi

# Create package directories
mkdir -p "${PACKAGE_DIR}/linux"
mkdir -p "${PACKAGE_DIR}/windows"

# Package Linux
LINUX_EXE="${LINUX_BUILD}/${PROJECT_NAME}"
if [ -f "$LINUX_EXE" ]; then
    echo "Packaging Linux..."
    cp "$LINUX_EXE" "${PACKAGE_DIR}/linux/"
    cp -r "$ASSETS_DIR" "${PACKAGE_DIR}/linux/"
else
    echo "Warning: Linux executable not found at $LINUX_EXE"
fi

# Package Windows
WINDOWS_EXE="${WINDOWS_BUILD}/${PROJECT_NAME}.exe"
if [ -f "$WINDOWS_EXE" ]; then
    echo "Packaging Windows..."
    cp "$WINDOWS_EXE" "${PACKAGE_DIR}/windows/"
    cp -r "$ASSETS_DIR" "${PACKAGE_DIR}/windows/"
else
    echo "Warning: Windows executable not found at $WINDOWS_EXE"
fi

echo ""
echo "Creating archives..."

cd "$PACKAGE_DIR"

if [ -f "linux/${PROJECT_NAME}" ]; then
    if command -v zip &> /dev/null; then
        zip -r "${PROJECT_NAME}-linux.zip" linux/
    else
        echo "Warning: 'zip' not found. Skipping linux archive."
    fi
fi

if [ -f "windows/${PROJECT_NAME}.exe" ]; then
    if command -v zip &> /dev/null; then
        zip -r "${PROJECT_NAME}-windows.zip" windows/
    else
        echo "Warning: 'zip' not found. Skipping windows archive."
    fi
fi

cd "$SCRIPT_DIR"

echo ""
echo "=== Packaging Complete ==="
echo "Output: ${PACKAGE_DIR}"
