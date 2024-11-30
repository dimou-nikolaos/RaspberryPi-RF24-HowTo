#!/bin/bash

# RF24 Library Installation Script

# Exit on any error
set -e

# Update package lists
echo "Updating package lists..."
sudo apt update

# Upgrade existing packages
echo "Upgrading installed packages..."
sudo apt upgrade -y

# Install required dependencies
echo "Installing git and cmake..."
sudo apt-get install -y git cmake

# Remove any existing RF24 library directory
if [ -d "$HOME/RF24" ]; then
    echo "Removing existing RF24 directory..."
    rm -rf "$HOME/RF24"
fi

# Clone RF24 library
echo "Cloning RF24 library..."
cd ~
git clone https://github.com/nRF24/RF24.git

# Build and install
cd RF24
mkdir -p build
cd build
cmake ..
sudo make install

echo "RF24 library installation complete!"
