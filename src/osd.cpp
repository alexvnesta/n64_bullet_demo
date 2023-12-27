#include "osd.hpp"
#include <libdragon.h>

// Variables for handling FPS
double frame_start;
double frame_end;
double frame_duration;
// Variables for handling Frame timing
const int n_frames_store_starts = 50; // Number of frames to store for averaging
unsigned long frame_starts[n_frames_store_starts] = {0};
unsigned long frame_ends[n_frames_store_starts] = {0};
int cur_frame_ring_i = 0; // Current index in the ring buffer
bool frame_starts_initialized = false;
// Variables for analytics chart
const int chart_length = 300; // Length of the chart (number of data points)
float fps_history[chart_length] = {0};
float processing_ms_history[chart_length] = {0};
int chart_index = 0; // Current index for storing data
double frame_duration_ms = 0;

void drawFPS(surface_t *localDisplay)
{
    unsigned long total_processing_time = 0;
    for (int i = 0; i < n_frames_store_starts; i++)
    {
        total_processing_time += frame_ends[i] - frame_starts[i];
    }
    double average_processing_ms_per_frame = ((double)total_processing_time / n_frames_store_starts) / (TICKS_PER_SECOND / 1000.0);
    float fps = 1000.0f / average_processing_ms_per_frame;

    // Draw the FPS
    char fps_str[100];
    snprintf(fps_str, sizeof(fps_str), "FPS:%.2f", fps);
    graphics_set_color(GRAPHICS_COLOR_GREEN, GRAPHICS_COLOR_TRANSPARENT);
    graphics_draw_text(localDisplay, 5, 5, fps_str);

    // Draw the average processing time
    char avg_time_str[100];
    snprintf(avg_time_str, sizeof(avg_time_str), "Avg Time:%.2f ms", average_processing_ms_per_frame);
    graphics_set_color(GRAPHICS_COLOR_RED, GRAPHICS_COLOR_TRANSPARENT);
    graphics_draw_text(localDisplay, 5, 15, avg_time_str);
}

void drawLineChart(surface_t *surf)
{
    int chart_start_x = 10;  // X position to start the chart
    int chart_start_y = 100; // Y position to start the chart
    int chart_height = 100;  // Height of the chart

    // Calculate average FPS and average processing time
    float total_fps = 0, total_processing_ms = 0;
    for (int i = 0; i < chart_length; i++)
    {
        total_fps += fps_history[i];
        total_processing_ms += processing_ms_history[i];
    }
    float avg_fps = total_fps / chart_length;
    float avg_processing_ms = total_processing_ms / chart_length;

    // Draw baselines for average FPS and average processing time
    int avg_fps_y = chart_start_y + chart_height - (int)(avg_fps / 2);
    int avg_processing_ms_y = chart_start_y + chart_height - (int)avg_processing_ms;
    graphics_draw_line_trans(surf, chart_start_x, avg_fps_y, chart_start_x + chart_length, avg_fps_y, GRAPHICS_COLOR_GREY);
    graphics_draw_line_trans(surf, chart_start_x, avg_processing_ms_y, chart_start_x + chart_length, avg_processing_ms_y, GRAPHICS_COLOR_GREY);

    for (int i = 1; i < chart_length; i++)
    {
        // Draw FPS line
        int x0 = chart_start_x + i - 1;
        int y0 = chart_start_y + chart_height - (int)(fps_history[i - 1] / 2); // Scale FPS for chart height
        int x1 = chart_start_x + i;
        int y1 = chart_start_y + chart_height - (int)(fps_history[i] / 2);
        graphics_draw_line(surf, x0, y0, x1, y1, GRAPHICS_COLOR_GREEN);

        // Draw processing time line
        y0 = chart_start_y + chart_height - (int)processing_ms_history[i - 1]; // Scale processing time
        y1 = chart_start_y + chart_height - (int)processing_ms_history[i];
        graphics_draw_line(surf, x0, y0, x1, y1, GRAPHICS_COLOR_RED);
    }
}

void updateFrameTiming()
{
    frame_end = get_ticks();
    frame_duration = frame_end - frame_start;

    frame_starts[cur_frame_ring_i] = frame_start;
    frame_ends[cur_frame_ring_i] = frame_end;
    cur_frame_ring_i = (cur_frame_ring_i + 1) % n_frames_store_starts;
    frame_starts_initialized = frame_starts_initialized || (cur_frame_ring_i == 0);

    fps_history[chart_index] = display_get_fps();

    // Add a check to avoid division by zero
    if (frame_duration != 0)
    {
        processing_ms_history[chart_index] = (double)frame_duration / (TICKS_PER_SECOND / 1000.0);
    }

    chart_index = (chart_index + 1) % chart_length;
}

void controlFrameRate()
{
    if (frame_duration != 0)
    {
        frame_duration_ms = (double)frame_duration / (TICKS_PER_SECOND / 1000);
    }
    printf("Frame start (milliSec): %f\n", frame_start / (TICKS_PER_SECOND / 1000));
    printf("Frame end (milliSec): %f\n", frame_end / (TICKS_PER_SECOND / 1000));
    printf("Frame duration (milliSec): %f\n", frame_duration_ms);
    if (frame_duration_ms < TARGET_FRAME_TIME_MS)
    {
        wait_ms(TARGET_FRAME_TIME_MS - frame_duration_ms);
    }
}

void drawControls(surface_t *localDisplay)
{
    // Message on screen for controls
    graphics_set_color(GRAPHICS_COLOR_WHITE, GRAPHICS_COLOR_TRANSPARENT);
    graphics_draw_text(localDisplay, 5, 25, "Controls:");
    graphics_draw_text(localDisplay, 5, 35, "Analog Stick: Move Plane");
    graphics_draw_text(localDisplay, 5, 45, "A: Reset Prisms");
}