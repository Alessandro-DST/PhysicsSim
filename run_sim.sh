#!/bin/bash
set -e

# Build
rm -rf build
mkdir -p build
cd build
cmake ..
cmake --build . -- -j$(nproc)

# Run simulation
./PhysicsSim
