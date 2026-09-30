# Build file

#!/usr/bin/env bash
set -e

# Build the C++ module
mkdir -p build
cd build
cmake ..
cmake --build .

# Go back to project root
cd ..

# Copy the compiled Python extension next to your Python script
cp build/*/package_scanner*.pyd .

# Run your Python script
python pypi_collector.py packages.txt

