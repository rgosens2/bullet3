#!/bin/sh

# Remove old cache
if [ -e CMakeCache.txt ]; then
  rm CMakeCache.txt
fi

# Create build directory
mkdir -p build_cmake
cd build_cmake

# Configure CMake for arm64 macOS
cmake -DBUILD_PYBULLET=ON \
      -DBUILD_PYBULLET_NUMPY=ON \
      -DUSE_DOUBLE_PRECISION=ON \
      -DBT_USE_EGL=ON \
      -DBUILD_OPENGL3_DEMOS=ON \
      -DBUILD_EXTRAS=ON \
      -DCMAKE_BUILD_TYPE=Release \
      -DCMAKE_OSX_ARCHITECTURES=arm64 \
      .. || exit 1 # NOTE: .. because we are in build_cmake and need to go up one level

echo "\nEn maken maar...\n"

# Build using all logical CPUs
make -j $(sysctl -n hw.logicalcpu) || exit 1

# Set up Python bindings
cd ../examples/pybullet
if [ -e pybullet.dylib ]; then
  ln -f -s pybullet.dylib pybullet.so
fi
rm -f pybullet_envs pybullet_data pybullet_utils
ln -s ../../../examples/pybullet/gym/pybullet_envs .
ln -s ../../../examples/pybullet/gym/pybullet_data .
ln -s ../../../examples/pybullet/gym/pybullet_utils .

echo "Completed build of Bullet."
