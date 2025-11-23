#!/bin/bash
# Prism Launcher with Mod Groups - Installation Script

set -e

echo "================================================"
echo "Prism Launcher with Mod Groups - Installer"
echo "================================================"
echo ""

# Check if running on Arch-based system
if ! command -v pacman &> /dev/null; then
    echo "Warning: This script is designed for Arch Linux and derivatives."
    echo "Continuing anyway..."
fi

# Install dependencies
echo "Installing dependencies..."
sudo pacman -S --needed base-devel cmake git qt5-base qt5-svg qt5-tools jdk17-openjdk extra-cmake-modules scdoc

# Clone repository
echo ""
echo "Cloning repository..."
if [ -d "PrismLauncher" ]; then
    echo "Directory exists, updating..."
    cd PrismLauncher
    git pull
else
    git clone https://github.com/quinnrallen-hub/PrismLauncher.git
    cd PrismLauncher
fi

# Initialize submodules
echo ""
echo "Initializing submodules..."
git submodule update --init --recursive

# Build
echo ""
echo "Building Prism Launcher..."
mkdir -p build
cd build

cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DLauncher_BUILD_PLATFORM=archlinux \
    -DLauncher_QT_VERSION_MAJOR=5 \
    -DENABLE_LTO=ON

echo ""
echo "Compiling... This may take a while."
make -j$(nproc)

# Install
echo ""
echo "Installing..."
sudo make install

echo ""
echo "================================================"
echo "Installation complete!"
echo "================================================"
echo ""
echo "You can now run: prismlauncher"
echo ""
echo "New feature: Mod Groups"
echo "  - Go to any instance's Mods page"
echo "  - Click 'Manage Groups' to create mod collections"
echo "  - Easily switch between different mod sets!"
echo ""
