#include <libdragon.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/gl_integration.h>
#include "camera.hpp"
#include "bulletClass.hpp"
#include "main.hpp"

#define TARGET_FRAME_TIME_MS 33.33                                         // milliseconds for 30 FPS
#define TARGET_FRAME_TIME (TARGET_FRAME_TIME_MS * TICKS_PER_SECOND / 1000) // converting milliseconds to ticks - TICKS_PER_SECOND is defined in libdragon

// Set up the Scene
float sceneRotation = 0.0f;
surface_t zbuffer;
camera_t camera = {100, 0};

// Allocate the prism display list
static GLuint prismDisplayList;

// Enstantiate the physics object
PhysicsObjectClass enstantiatedPhysicsObject;

// Initialize plane rotation
float plane_rotationX = 0.0f;
float plane_rotationZ = 0.0f;

// Variables for handling FPS
double frame_start;
double frame_end;
double frame_duration;

void render()
{
    // Framerate handling
    frame_start = get_ticks();

    surface_t *disp = display_get();
    rdpq_attach(disp, &zbuffer);

    gl_context_begin();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Clear color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    camera_transform(&camera);
    glRotatef(90, 0.0f, 1.0f, 0.0f);

    // Rotate the scene
    // sceneRotation += 0.2f;
    glRotatef(sceneRotation, 0.0f, 1.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);

    // Step the simulation - varies based on FPS
    enstantiatedPhysicsObject.stepSimulation();

    drawPlane();

    drawPrismFromDisplayList(&enstantiatedPhysicsObject);

    gl_context_end();

    rdpq_detach_wait(); // Wait for the RDP queue to finish otherwise the display won't properly handle the text

    // Handle FPS
    drawFPS(disp);
    frame_end = get_ticks();
    frame_duration = frame_end - frame_start;
    double frame_duration_ms = (double)frame_duration / (TICKS_PER_SECOND / 1000);
    printf("Frame start (milliSec): %f\n", frame_start / (TICKS_PER_SECOND / 1000));
    printf("Frame end (milliSec): %f\n", frame_end / (TICKS_PER_SECOND / 1000));
    printf("Frame duration (milliSec): %f\n", frame_duration_ms);
    if (frame_duration_ms < TARGET_FRAME_TIME_MS)

    {
        printf("Sleeping for %f\n", TARGET_FRAME_TIME_MS - frame_duration_ms);
        wait_ms(TARGET_FRAME_TIME_MS - frame_duration_ms);
    }

    // Don't forget to update the display
    display_show(disp);
}

void setup()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)display_get_width() / (float)display_get_height(), 50.0f, 400.0f);
    zbuffer = surface_alloc(FMT_RGBA16, display_get_width(), display_get_height());

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    camera.distance = -200;
    camera.rotationY = -22.5;

    // Set up the prism display list - this is a static object so we only need to do this once even if we draw it multiple times
    setupPrismDisplayList();

    // Create the physics object
    enstantiatedPhysicsObject.initializePhysics();
    enstantiatedPhysicsObject.createPrismRigidBody();
    enstantiatedPhysicsObject.createGroundRigidBody();
}

void drawPlane() // The plane will act as the ground
{
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);

    // Rotate the plane if we want to
    glRotatef(plane_rotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(plane_rotationZ, 0.0f, 0.0f, 1.0f);

    // Update the physics object with the plane's rotation
    enstantiatedPhysicsObject.updatePlaneRotation(plane_rotationX, plane_rotationZ);

    glBegin(GL_QUADS);
    // Making each vertex a different color for demonstration purposes
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(-100.0f, 0.0f, -100.0f);
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-100.0f, 0.0f, 100.0f);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(100.0f, 0.0f, 100.0f);
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(100.0f, 0.0f, -100.0f);
    glEnd();
    glPopMatrix();
}

void setupPrismDisplayList()
{
    // Allocate the prism display list
    prismDisplayList = glGenLists(1);

    // Create the prism display list
    glNewList(prismDisplayList, GL_COMPILE);
    glBegin(GL_TRIANGLES);
    // Front
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f, 9.0f, 0.0f);
    glVertex3f(-3.0f, 0.0f, 3.0f);
    glVertex3f(3.0f, 0.0f, 3.0f);
    // Right
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 9.0f, 0.0f);
    glVertex3f(3.0f, 0.0f, 3.0f);
    glVertex3f(3.0f, 0.0f, -3.0f);
    // Back
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 9.0f, 0.0f);
    glVertex3f(3.0f, 0.0f, -3.0f);
    glVertex3f(-3.0f, 0.0f, -3.0f);
    // Left
    glColor3f(1.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, 9.0f, 0.0f);
    glVertex3f(-3.0f, 0.0f, -3.0f);
    glVertex3f(-3.0f, 0.0f, 3.0f);
    glEnd();
    glEndList();
}

void drawPrismFromDisplayList(PhysicsObjectClass *localPhysicsObject)
{
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);

    btVector3 position = localPhysicsObject->getPrismRigidBodyPosition();
    // Apply the physics transform to the prism
    glTranslatef(position.getX(), position.getY(), position.getZ());

    // Apply the physics rotation to the prism
    btQuaternion rotation = localPhysicsObject->getPrismRigidBodyRotation();
    printf("Rotation: %f, %f, %f, %f\n", rotation.getAngle() * 100, rotation.getX(), rotation.getY(), rotation.getZ());
    //  Translate the quaternion to a rotation matrix
    glRotatef(rotation.getAngle() * 100, rotation.getX(), rotation.getY(), rotation.getZ());

    glCallList(prismDisplayList);
    glPopMatrix();
}

int main()
{
    // Initialize debug console
    debug_init_isviewer();
    console_init();
    console_set_render_mode(RENDER_MANUAL);
    console_set_debug(1);

    joypad_init();

    // Setting up the display and GL
    display_init(RESOLUTION_320x240, DEPTH_16_BPP, 3, GAMMA_NONE, FILTERS_RESAMPLE_ANTIALIAS_DEDITHER);
    rdpq_init();
    gl_init();

    // Setting up the camera
    setup();

    // continuously loop through the render function
    while (1)
    {
        handleControls();
        render();
    }

    printf("Intro finished\n");
    gl_close();
    rdpq_close();
    display_close();
}

// Controls to move the plane around
void handleControls()
{
    joypad_poll();

    int vertical_input = joypad_get_axis_held(JOYPAD_PORT_1, JOYPAD_AXIS_STICK_Y);
    int horizontal_input = joypad_get_axis_held(JOYPAD_PORT_1, JOYPAD_AXIS_STICK_X);
    joypad_buttons_t button_pressed = joypad_get_buttons_pressed(JOYPAD_PORT_1);

    plane_rotationX += horizontal_input * 0.3;
    plane_rotationZ -= vertical_input * 0.3;

    if (button_pressed.a)
    {
        enstantiatedPhysicsObject.resetPrismRigidBody();
    }
}

void drawFPS(surface_t *localDisplay)
{
    // Get the current FPS
    float fps = display_get_fps();

    // Print the FPS to the screen
    graphics_draw_text(localDisplay, 5, 5, std::to_string(fps).c_str());
}