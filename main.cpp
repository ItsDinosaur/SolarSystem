#include <GL/glut.h>
#include <cmath>
#include <string>
#include <cstdlib>
#include "body.h"
#include "physics.h"
#include "camera.h"



// --------- OpenGL for rendering below
void drawPlanet(const body& b) {
    glPushMatrix();
    glTranslatef(b.pos[0], b.pos[1], b.pos[2]);

    double visualR;
    if (b.name == "Sun") {

        visualR = 0.2;
    } else {

        visualR = 0.03 + (b.radius / 70000.0) * 0.04;
    }

    glutSolidSphere(visualR, 32, 32);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    updateCamera();

    gluLookAt(camera_eye[0], camera_eye[1], camera_eye[2],
              camera_center[0], camera_center[1], camera_center[2],
              camera_up[0], camera_up[1], camera_up[2]);

    // RYSOWANIE LINII ORBITALNYCH
    if (showOrbits) {
        glDisable(GL_LIGHTING);
        glColor3f(0.2f, 0.2f, 0.2f);

        for (int i = 1; i < NUM_BODIES; ++i) {
            // Promień orbity = odległość od Słońca (ciało 0) na płaszczyźnie XZ
            double dx = bodies[i].pos[0] - bodies[0].pos[0];
            double dz = bodies[i].pos[2] - bodies[0].pos[2];
            double radius = std::sqrt(dx*dx + dz*dz);

            glBegin(GL_LINE_LOOP);
            for (int j = 0; j < 100; ++j) {
                double angle = 2.0 * M_PI * j / 100.0;
                glVertex3f(
                    bodies[0].pos[0] + cos(angle) * radius,
                    bodies[0].pos[1],
                    bodies[0].pos[2] + sin(angle) * radius
                );
            }
            glEnd();
        }
    }

    // KONFIGURACJA ŚWIATŁA
    GLfloat light_pos[] = {
        (GLfloat)bodies[0].pos[0],
        (GLfloat)bodies[0].pos[1],
        (GLfloat)bodies[0].pos[2],
        1.0f
    };


    GLfloat diffuse_light[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat ambient_light[] = {0.1f, 0.1f, 0.1f, 1.0f};

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);

    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);

    // RYSOWANIE CIAŁ NIEBIESKICH
    for (int i = 0; i < NUM_BODIES; ++i) {
        if (bodies[i].name == "Sun") {
            glDisable(GL_LIGHTING);
            glColor3f(1.0f, 0.8f, 0.0f);
        } else {
            glEnable(GL_LIGHTING);
            if (bodies[i].name == "Mercury")      glColor3f(0.6f, 0.6f, 0.6f);
            else if (bodies[i].name == "Venus")   glColor3f(0.8f, 0.6f, 0.4f);
            else if (bodies[i].name == "Earth")   glColor3f(0.2f, 0.5f, 1.0f);
            else if (bodies[i].name == "Mars")    glColor3f(0.9f, 0.3f, 0.1f);
            else if (bodies[i].name == "Jupiter") glColor3f(0.8f, 0.5f, 0.3f);
            else if (bodies[i].name == "Saturn")  glColor3f(0.9f, 0.8f, 0.5f);
            else if (bodies[i].name == "Uranus")  glColor3f(0.5f, 0.8f, 0.9f);
            else if (bodies[i].name == "Neptune") glColor3f(0.2f, 0.3f, 0.8f);
            else                                  glColor3f(1.0f, 1.0f, 1.0f);
        }
        drawPlanet(bodies[i]);
    }

    glEnable(GL_LIGHTING);
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(unsigned char key, int x, int y) {
    if (key == 27) // ESC so we shall quit
        exit(0);
}

void timer(int value) {
    updatePhysics();

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

int main(int argc, char** argv) {
    initBodies();
    initCamera();

    glutInit(&argc, argv);


    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1280, 720); // Rozdzielczość startowa
    glutCreateWindow("SolarSystem");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // Callbacki kamery i sterowania
    glutKeyboardFunc(handleCameraKeyboard);
    glutSpecialFunc(handleCameraSpecial);
    glutMouseFunc(handleCameraMouse);

    glutTimerFunc(0, timer, 0);
    glutMainLoop();

    return 0;
}