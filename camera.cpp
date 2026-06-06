#include "camera.h"
#include "body.h"
#include <GL/glut.h>
#include <cmath>

bool showOrbits = true; // Definicja flagi

namespace {
    int trackedBodyIndex = 0;

    // Zmienione na potężne odległości, żeby widzieć zewnętrzne planety!
    double rel_eye[3] = {0.0, 40.0, 50.0};
}

void initCamera() {
    camera_up[0] = 0.0;
    camera_up[1] = 1.0;
    camera_up[2] = 0.0;

    trackedBodyIndex = 0;
    rel_eye[0] = 0.0; rel_eye[1] = 40.0; rel_eye[2] = 50.0;
}

int getTrackedBodyIndex() {
    return trackedBodyIndex;
}

void updateCamera() {
    if (trackedBodyIndex >= 0 && trackedBodyIndex < NUM_BODIES) {

        camera_center[0] = bodies[trackedBodyIndex].pos[0];
        camera_center[1] = bodies[trackedBodyIndex].pos[1];
        camera_center[2] = bodies[trackedBodyIndex].pos[2];

        if (trackedBodyIndex == 0) { //slonce

            camera_eye[0] = camera_center[0] + rel_eye[0];
            camera_eye[1] = camera_center[1] + rel_eye[1];
            camera_eye[2] = camera_center[2] + rel_eye[2];
        } else {

            double ax = bodies[trackedBodyIndex].pos[0] - bodies[0].pos[0];
            double ay = bodies[trackedBodyIndex].pos[1] - bodies[0].pos[1];
            double az = bodies[trackedBodyIndex].pos[2] - bodies[0].pos[2];


            double aLen = std::sqrt(ax*ax + ay*ay + az*az);
            if (aLen > 0.0) { ax /= aLen; ay /= aLen; az /= aLen; }
            else { ax = 0.0; ay = 0.0; az = 1.0; }

            double rx = az;
            double ry = 0.0;
            double rz = -ax;


            double rLen = std::sqrt(rx*rx + rz*rz);
            if (rLen > 0.0) { rx /= rLen; rz /= rLen; }


            camera_eye[0] = camera_center[0] + rx * rel_eye[0] + 0.0 * rel_eye[1] + ax * rel_eye[2];
            camera_eye[1] = camera_center[1] + ry * rel_eye[0] + 1.0 * rel_eye[1] + ay * rel_eye[2];
            camera_eye[2] = camera_center[2] + rz * rel_eye[0] + 0.0 * rel_eye[1] + az * rel_eye[2];
        }
    }
}

void handleCameraKeyboard(unsigned char key, int x, int y) {
    if (key == 27) // ESC kończy program
        exit(0);

    // TOGGLE ORBIT (SPACJA)
    if (key == ' ') {
        showOrbits = !showOrbits;
    }

    // Klawisze 1 do 8 przełączają planety (Merkury = 1, Wenus = 2, ..., Neptun = 8)
    if (key >= '1' && key <= '8') {
        int planetIndex = key - '0';
        if (planetIndex < NUM_BODIES) {
            trackedBodyIndex = planetIndex;
            
            // Automatyczne dopasowanie odległości, żeby planeta nie była za mała lub za duża. Raczej chodzi o to zeby to jakos sie dało oglądać. Moze bedzie trzeba zmienic
            if (planetIndex <= 4) { // Planety skaliste
                rel_eye[0] = 0.0; rel_eye[1] = 0.15; rel_eye[2] = 0.35;
            } else {
                rel_eye[0] = 0.0; rel_eye[1] = 0.5; rel_eye[2] = 0.9;
            }
        }
    }
    
    // Klawisz 0 wraca do ogólnego widoku na Słońce
    if (key == '0') {
        trackedBodyIndex = 0;
        rel_eye[0] = 0.0; rel_eye[1] = 4.0; rel_eye[2] = 6.0;
    }
    
    glutPostRedisplay();
}

void handleCameraSpecial(int key, int x, int y) {
    // Czułość strzałek zależy od tego, czy patrzymy na całe układ (Słońce), czy na małą planetę
    double step = (trackedBodyIndex == 0) ? 0.2 : 0.02;

    switch (key) {
        case GLUT_KEY_LEFT:  rel_eye[0] -= step; break;
        case GLUT_KEY_RIGHT: rel_eye[0] += step; break;
        case GLUT_KEY_UP:    rel_eye[1] += step; break;
        case GLUT_KEY_DOWN:  rel_eye[1] -= step; break;
    }
    glutPostRedisplay();
}

// Obsługa scrolla dla klasycznego GLUT / niektórych systemów (jako Button 3 i 4)
void handleCameraMouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        if (button == 3) {
            // Scroll w górę -> Przybliżenie (skalujemy wektor w dół)
            rel_eye[0] *= 0.9; rel_eye[1] *= 0.9; rel_eye[2] *= 0.9;
        }
        else if (button == 4) {
            // Scroll w dół -> Oddalenie (skalujemy wektor w górę)
            rel_eye[0] *= 1.1; rel_eye[1] *= 1.1; rel_eye[2] *= 1.1;
        }
        glutPostRedisplay();
    }
}

// Obsługa scrolla dla nowszego FreeGLUT (bardziej niezawodne na Windowsie)
void handleCameraMouseWheel(int wheel, int direction, int x, int y) {
    if (direction > 0) { // Przybliżenie
        rel_eye[0] *= 0.9; rel_eye[1] *= 0.9; rel_eye[2] *= 0.9;
    } else { // Oddalenie
        rel_eye[0] *= 1.1; rel_eye[1] *= 1.1; rel_eye[2] *= 1.1;
    }
    glutPostRedisplay();
}