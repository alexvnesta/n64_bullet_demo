# Bullet Physics with libdragon N64 SDK

This repository provides an example of using Bullet Physics with the N64 libdragon SDK OpenGL branch to create physics simulations on the Nintendo 64 console. It is not working perfectly, but I figured I would share it in case anyone else is interested in using Bullet Physics on the N64. I am not a Bullet Physics or Libdragon or C++ expert, so I am sure there are better ways to do things. I will try to update this repository as I learn more.

Check the docs folder for more information.

## Demo Video

![Physics Example](./physics_example.gif)

## Getting Started

To get started, follow these steps:

1. Clone the official Bullet Physics repository from [here](https://github.com/bulletphysics/bullet3). The latest commit should work. For reference, the commit used in this example is: 6bb8d1123d8a55d407b19fd3357c724d0f5c9d3c.

2. Configure the custom CMake toolchain to point to your libdragon SDK. The toolchain file `N64Toolchain.cmake` is included in this repository. Place it in the root of the Bullet Physics repository.

3. Run CMake to generate the build files:
```
    cmake -DCMAKE_TOOLCHAIN_FILE=N64Toolchain.cmake -DBUILD_UNIT_TESTS=OFF -DBUILD_BULLET3=OFF -DCMAKE_INSTALL_PREFIX:PATH=/opt/libdragon .
```