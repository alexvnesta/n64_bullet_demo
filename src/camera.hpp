#pragma once

#include <GL/gl_integration.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <libdragon.h>

typedef struct
{
    float distance;
    float rotationX;
    float rotationY;
} camera_t;

void camera_transform(const camera_t *camera)
{
    // Set the camera transform
    glLoadIdentity();
    gluLookAt(
        0, -camera->distance, -camera->distance,
        0, 0, 0,
        0, 1, 0);
    glRotatef(camera->rotationX, 0, 1, 0);
    glRotatef(camera->rotationY, 1, 0, 0);
}