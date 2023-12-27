# Bullet Engine
Obtaining the Bullet Physics Engine required some effort on my part to understand the process. Luckily, no additional code modifications were needed to the original Bullet codebase. I received valuable assistance from pioneers like @Hazematman. In this repository, I have documented the steps I followed to successfully integrate the Bullet Physics Engine into my N64 project. I hope this documentation will be helpful to others who are also working with the Bullet Physics Engine on the N64.


# Getting bullet
I got bullet from the official repo: [https://github.com/bulletphysics/bullet3](https://github.com/bulletphysics/bullet3) and the latest commit was compatible when I followed this procedure. For reference, the commit I used was: `6bb8d1123d8a55d407b19fd3357c724d0f5c9d3c`  
# Compiling bullet
After cloning, you will need to make a custom cmake toolchain that points to your libdragon sdk to compile bullet. I have included my toolchain file in this repo. The toolchain file is called N64Toolchain.cmake. Simply place this file in the root of the bullet repo and then run cmake:

See N64Toolchain.cmake for details on how to configure the toolchain. You will need to change the paths to point to your libdragon sdk. Once you have configured the toolchain, you can compile bullet with the following command:

[N64Toolchain.cmake](../N64Toolchain.cmake)

## N64Toolchain.cmake description
```
# Specify the C and C++ compilers
set(CMAKE_C_COMPILER /opt/libdragon/bin/mips64-elf-gcc)
set(CMAKE_CXX_COMPILER /opt/libdragon/bin/mips64-elf-g++)

# Specify the linker
set(CMAKE_LINKER /opt/libdragon/bin/mips64-elf-ld)

# Separate C and C++ flags
set(CMAKE_C_FLAGS "-march=vr4300 -mtune=vr4300 -mips3 -I/opt/libdragon/mips64-elf/include -DN64 -O2 -G0 -falign-functions=32 -ffunction-sections -fdata-sections -g -ffast-math -ftrapping-math -fno-associative-math -std=gnu99")
set(CMAKE_CXX_FLAGS "-march=vr4300 -mtune=vr4300 -mips3 -I/opt/libdragon/mips64-elf/include -DN64 -O2 -G0 -falign-functions=32 -ffunction-sections -fdata-sections -g -ffast-math -ftrapping-math -fno-associative-math -std=c++11 -fno-exceptions -fno-rtti")

# Verify and adjust linker flags
# set(CMAKE_EXE_LINKER_FLAGS "-L/opt/libdragon/mips64-elf/lib -ldragon -lm -ldragonsys -Tn64.ld")

set(CMAKE_EXE_LINKER_FLAGS "-Wl,-Map,output.map")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/opt/libdragon/mips64-elf/lib/ -lc -lm -T /opt/libdragon/mips64-elf/lib/n64.ld")


# Disable certain features not needed or supported on the N64
add_definitions(-D_DISABLE_THREADS -D_DISABLE_LIBSSP)

# Specify include and library directories
set(CMAKE_INCLUDE_PATH /opt/libdragon/include)
set(CMAKE_LIBRARY_PATH /opt/libdragon/lib)

# Specify the target architecture
set(CMAKE_SYSTEM_NAME Nintendo64)

# Set the default output paths for executables and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories and link directories
include_directories(${CMAKE_INCLUDE_PATH})
link_directories(${CMAKE_LIBRARY_PATH})

# Additional options (set according to your project requirements)
option(BUILD_EGL "Build OpenGL/EGL" OFF)
OPTION(BUILD_EXTRAS "Set when you want to build the extras" OFF)

OPTION(USE_DOUBLE_PRECISION "Use double precision"	OFF)
OPTION(USE_GRAPHICAL_BENCHMARK "Use Graphical Benchmark" OFF)
OPTION(BUILD_SHARED_LIBS "Use shared libraries" OFF)
OPTION(USE_SOFT_BODY_MULTI_BODY_DYNAMICS_WORLD "Use btSoftMultiBodyDynamicsWorld" OFF)

SET(CMAKE_BUILD_TYPE "Release")
OPTION(USE_MSVC_INCREMENTAL_LINKING "Use MSVC Incremental Linking" OFF)
OPTION(BUILD_CPU_DEMOS "Build original Bullet CPU examples" OFF)
OPTION(USE_OPENVR "Use OpenVR for virtual reality" OFF)
OPTION(ENABLE_VHACD "Use VHACD in BulletRobotics and pybullet" OFF)
OPTION(BULLET2_MULTITHREADING "Build Bullet 2 libraries with mutex locking around certain operations (required for multi-threading)" OFF)
OPTION(INTERNAL_UPDATE_SERIALIZATION_STRUCTURES "Internal update serialization structures" OFF)
OPTION(BUILD_BULLET3 "Set when you want to build Bullet 3" OFF)
OPTION(BUILD_ENET "Set when you want to build apps with enet UDP networking support" OFF)
OPTION(BUILD_CLSOCKET "Set when you want to build apps with enet TCP networking support" OFF)
OPTION(BUILD_PYBULLET "Set when you want to build pybullet (Python bindings for Bullet)" OFF)
OPTION(BUILD_BULLET2_DEMOS "Set when you want to build the Bullet 2 demos" OFF)
```

# Configuring the cmake toolchain:
```
cmake -DCMAKE_TOOLCHAIN_FILE=N64Toolchain.cmake -DBUILD_UNIT_TESTS=OFF -DBUILD_BULLET3=OFF -DCMAKE_INSTALL_PREFIX:PATH=/opt/libdragon .
```
Depending on where libdragon is installed, you may need to change the install prefix. The above command will compile bullet and install it to /opt/libdragon. You can change this to any directory you want.

# Compile and install bullet
```
make
make install
```

# Using bullet in your project Makefile
You will need to add the following to your Makefile:
```
BULLET_LIBS = -lLinearMath -lBulletCollision -lBulletDynamics
BULLET_INCLUDE = -I/opt/libdragon/include/bullet
```



To see another example and some physics implementations on n64, check the code in this repo by @Hazematman:

[Git - Hazematman/Bug-Game](https://github.com/Hazematman/Bug-Game/tree/master)

