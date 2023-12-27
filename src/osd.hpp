#pragma once

#include <libdragon.h>

#define GRAPHICS_COLOR_WHITE graphics_make_color(255, 255, 255, 255)
#define GRAPHICS_COLOR_RED graphics_make_color(255, 0, 0, 255)
#define GRAPHICS_COLOR_GREEN graphics_make_color(0, 255, 0, 255)
#define GRAPHICS_COLOR_BLUE graphics_make_color(0, 0, 255, 255)
#define GRAPHICS_COLOR_GREY graphics_make_color(128, 128, 128, 255)
#define GRAPHICS_COLOR_TRANSPARENT graphics_make_color(0, 0, 0, 0)

#define TARGET_FRAME_TIME_MS 33.33                                         // milliseconds for 30 FPS
#define TARGET_FRAME_TIME (TARGET_FRAME_TIME_MS * TICKS_PER_SECOND / 1000) // converting milliseconds to ticks - TICKS_PER_SECOND is defined in libdragon

// Function prototypes
void drawFPS(surface_t *localDisplay);
void drawLineChart(surface_t *surf);
void updateFrameTiming();
void controlFrameRate();
void drawControls(surface_t *localDisplay);

// Global variables
extern double frame_start;