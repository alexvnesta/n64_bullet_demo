#pragma once

void render();
void setup();
void drawPlane();
void handleControls();
void setupPrismDisplayList();
void drawPrismFromDisplayList(PhysicsObjectClass *localPhysicsObject);
void drawFPS(surface_t *localDisplay);
