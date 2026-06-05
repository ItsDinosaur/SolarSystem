#ifndef CAMERA_H
#define CAMERA_H

extern bool showOrbits;

void initCamera();
void updateCamera();
void handleCameraKeyboard(unsigned char key, int x, int y);
void handleCameraSpecial(int key, int x, int y);
void handleCameraMouse(int button, int state, int x, int y);

#endif