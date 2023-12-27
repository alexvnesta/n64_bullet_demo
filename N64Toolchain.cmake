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
