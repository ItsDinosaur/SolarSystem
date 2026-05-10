#include <GL/glut.h>
#include <cmath>
#include <string>
#include <cstdlib>
#include "body.h"
using namespace std;

// in AU^3 / (sun mass * day^2)
const double G = 0.0002959122082855911;

// time step
const double DT = 0.5;

// scale for gui so we see actually the planets :)
const double VISUAL_SCALE = 80.0; 

// --------- OpenGL for rendering below
void drawPlanet(const body& b) {
    glPushMatrix();
    glTranslatef(b.pos[0], b.pos[1], b.pos[2]);
    double visualR = b.radius * VISUAL_SCALE;
    glutSolidSphere(visualR, 32, 32);
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(20.0, 30.0, 40.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    GLfloat light_pos[] = {0.0f, 10.0f, 20.0f, 1.0f};
    GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    for (int i = 0; i < NUM_BODIES; ++i) {
        // maybe color or textures? but thats for later
        glColor3f(1.0f, 0.8f, 0.2f);
        drawPlanet(bodies[i]);
    }

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
    // TODO update physics
    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

// and then there was chaos
int main(int argc, char** argv) {
    initBodies();

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1200, 800);
    glutCreateWindow("SolarSystem - sim the only you need");

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glShadeModel(GL_SMOOTH);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0, timer, 0);

    glutMainLoop();
    return 0;
}