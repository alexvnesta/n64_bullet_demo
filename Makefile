N64_INST := /opt/libdragon
N64_EMULATOR := /Applications/ares.app/Contents/MacOS/ares
ROM_NAME := BulletDemo

$(info $$N64_INST directory is: [${N64_INST}])

BUILD_DIR=build
include $(N64_INST)/include/n64.mk

## Ad the .h files to the include path, specifically src/asset/cube.h
INCLUDE+=-I/opt/libdragon/include -Iinclude -Isrc
MAPCMD+=-Map
CFLAGS+=-I/opt/libdragon/include -I/opt/libdragon/include/bullet -std=c11
CXXFLAGS+=-Isrc -I/opt/libdragon/include -I/opt/libdragon/include/bullet -std=c++14
LDFLAGS+=-g -lstdc++ -L/opt/libdragon/lib -ldragon -lc -lm -ldragonsys,--start-group -lBullet3Common -lBulletDynamics -lBulletCollision -lBulletInverseDynamics -lBulletSoftBody -lLinearMath,--end-group,--gc-sections

# add the header files
header = $(wildcard src/*.hpp)

src = $(wildcard src/*.cpp) 

all: $(ROM_NAME).z64

$(BUILD_DIR)/$(ROM_NAME).elf: $(src:%.cpp=$(BUILD_DIR)/%.o)

$(ROM_NAME).z64: N64_ROM_TITLE="$(ROM_NAME)"

run: $(ROM_NAME).z64
	$(N64_EMULATOR) $(ROM_NAME).z64

clean:
	rm -rf $(BUILD_DIR) filesystem $(ROM_NAME).z64

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean