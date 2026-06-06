#include <GL/glut.h>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <string>
#include <utility>
#include <vector>

#include "body.h"
#include "body_ui.h"
#include "camera.h"
#include "physics.h"

using namespace std;

namespace {
int windowWidth = 1280;
int windowHeight = 720;
bool sliderDragging = false;

const double sliderMinDt = 0.01;
const double sliderMaxDt = 1.5;
const int hudMargin = 22;

const int speedPanelWidth = 290;
const int speedPanelHeight = 72;
const int speedTrackX = 18;
const int speedTrackY = 22;
const int speedTrackWidth = 250;
const int speedTrackHeight = 10;

vector<double> orbitRadii;

double clampDouble(double value, double minValue, double maxValue) {
    return max(minValue, min(value, maxValue));
}

double sliderT() {
    if (sliderMaxDt <= sliderMinDt) {
        return 0.0;
    }
    return (DT - sliderMinDt) / (sliderMaxDt - sliderMinDt);
}

double dtFromSliderX(int mouseX) {
    double t = static_cast<double>(mouseX - hudMargin - speedTrackX) / static_cast<double>(speedTrackWidth);
    t = clampDouble(t, 0.0, 1.0);
    return sliderMinDt + t * (sliderMaxDt - sliderMinDt);
}

bool pointInRect(int x, int y, int left, int bottom, int width, int height) {
    return x >= left && x <= left + width && y >= bottom && y <= bottom + height;
}

void initOrbitRadii() {
    orbitRadii.clear();
    if (NUM_BODIES <= 1) {
        return;
    }

    orbitRadii.resize(NUM_BODIES, 0.0);
    for (int i = 1; i < NUM_BODIES; ++i) {
        double dx = bodies[i].pos[0] - bodies[0].pos[0];
        double dz = bodies[i].pos[2] - bodies[0].pos[2];
        orbitRadii[i] = sqrt(dx * dx + dz * dz);
    }
}

void drawBitmapText(void* font, const std::string& text, int x, int y) {
    glRasterPos2i(x, y);
    for (size_t i = 0; i < text.size(); ++i) {
        glutBitmapCharacter(font, text[i]);
    }
}

void drawFilledRect(int x, int y, int width, int height, float r, float g, float b, float a) {
    glColor4f(r, g, b, a);
    glBegin(GL_QUADS);
    glVertex2i(x, y);
    glVertex2i(x + width, y);
    glVertex2i(x + width, y + height);
    glVertex2i(x, y + height);
    glEnd();
}

void drawBorderRect(int x, int y, int width, int height, float r, float g, float b, float a) {
    glColor4f(r, g, b, a);
    glBegin(GL_LINE_LOOP);
    glVertex2i(x, y);
    glVertex2i(x + width, y);
    glVertex2i(x + width, y + height);
    glVertex2i(x, y + height);
    glEnd();
}

body* focusedBody() {
    int index = getTrackedBodyIndex();
    if (index < 0 || index >= NUM_BODIES) {
        if (NUM_BODIES == 0) {
            return 0;
        }
        index = 0;
    }
    return &bodies[index];
}

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

void drawOrbitGuides() {
    if (orbitRadii.size() != static_cast<size_t>(NUM_BODIES) || NUM_BODIES <= 1) {
        return;
    }

    glPushAttrib(GL_ENABLE_BIT | GL_LINE_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);
    glLineWidth(1.5f);
    glColor3f(0.2f, 0.2f, 0.2f);

    for (int i = 1; i < NUM_BODIES; ++i) {
        glBegin(GL_LINE_LOOP);
        for (int j = 0; j < 100; ++j) {
            double angle = 2.0 * M_PI * j / 100.0;
            glVertex3f(
                bodies[0].pos[0] + cos(angle) * orbitRadii[i],
                bodies[0].pos[1],
                bodies[0].pos[2] + sin(angle) * orbitRadii[i]
            );
        }
        glEnd();
    }

    glDepthMask(GL_TRUE);
    glPopAttrib();
}

void drawHudOverlay() {
    body* current = focusedBody();
    if (current == 0) {
        return;
    }

    const body* sunBody = (NUM_BODIES > 0) ? &bodies[0] : 0;
    vector<pair<string, string> > lines = getHudLines(*current, sunBody);

    int panelWidth = 380;
    int lineHeight = 22;
    int panelHeight = 28 + static_cast<int>(lines.size()) * lineHeight + 18;
    int panelX = windowWidth - hudMargin - panelWidth;
    int panelY = hudMargin;

    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    drawFilledRect(panelX, panelY, panelWidth, panelHeight, 0.05f, 0.08f, 0.12f, 0.72f);
    drawBorderRect(panelX, panelY, panelWidth, panelHeight, 1.0f, 1.0f, 1.0f, 0.16f);

    int textX = panelX + 16;
    int textY = panelY + panelHeight - 24;

    glColor3f(1.0f, 0.88f, 0.3f);
    drawBitmapText(GLUT_BITMAP_TIMES_ROMAN_24, current->name, textX, textY);
    textY -= 28;

    glColor3f(0.92f, 0.94f, 0.98f);
    for (size_t i = 0; i < lines.size(); ++i) {
        drawBitmapText(GLUT_BITMAP_HELVETICA_18, lines[i].first + ": " + lines[i].second, textX, textY);
        textY -= lineHeight;
    }

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glPopAttrib();
}

void drawSpeedSlider() {
    int panelX = hudMargin;
    int panelY = hudMargin;
    int trackX = panelX + speedTrackX;
    int trackY = panelY + speedTrackY;
    double normalized = clampDouble(sliderT(), 0.0, 1.0);
    int knobX = trackX + static_cast<int>(normalized * speedTrackWidth);
    int knobWidth = 14;
    int knobHeight = 24;

    glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    drawFilledRect(panelX, panelY, speedPanelWidth, speedPanelHeight, 0.08f, 0.08f, 0.08f, 0.62f);
    drawBorderRect(panelX, panelY, speedPanelWidth, speedPanelHeight, 1.0f, 1.0f, 1.0f, 0.14f);

    glColor3f(0.94f, 0.94f, 0.94f);
    drawBitmapText(GLUT_BITMAP_HELVETICA_18, "Time step", panelX + 16, panelY + speedPanelHeight - 22);

    string valueText = string("DT ") + formatDtDisplay(DT);
    glColor3f(0.75f, 0.9f, 1.0f);
    drawBitmapText(GLUT_BITMAP_HELVETICA_18, valueText, panelX + 124, panelY + speedPanelHeight - 22);

    drawFilledRect(trackX, trackY, speedTrackWidth, speedTrackHeight, 0.18f, 0.18f, 0.2f, 0.95f);
    drawFilledRect(trackX, trackY, static_cast<int>(normalized * speedTrackWidth), speedTrackHeight, 0.18f, 0.55f, 0.95f, 0.85f);
    drawBorderRect(trackX, trackY, speedTrackWidth, speedTrackHeight, 0.9f, 0.9f, 0.9f, 0.15f);
    drawFilledRect(knobX - knobWidth / 2, trackY - 7, knobWidth, knobHeight, 0.96f, 0.96f, 0.98f, 0.95f);
    drawBorderRect(knobX - knobWidth / 2, trackY - 7, knobWidth, knobHeight, 0.2f, 0.2f, 0.25f, 0.85f);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);

    glPopAttrib();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    updateCamera();

    gluLookAt(camera_eye[0], camera_eye[1], camera_eye[2],
              camera_center[0], camera_center[1], camera_center[2],
              camera_up[0], camera_up[1], camera_up[2]);

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
            else                                    glColor3f(1.0f, 1.0f, 1.0f);
        }
        drawPlanet(bodies[i]);
    }

    if (showOrbits) {
        drawOrbitGuides();
    }

    drawHudOverlay();
    drawSpeedSlider();

    glEnable(GL_LIGHTING);
    glutSwapBuffers();
}

void reshape(int w, int h) {
    windowWidth = w;
    windowHeight = h > 0 ? h : 1;

    glViewport(0, 0, windowWidth, windowHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, static_cast<double>(windowWidth) / static_cast<double>(windowHeight), 0.1, 200.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int value) {
    updatePhysics();

    glutPostRedisplay();
    glutTimerFunc(16, timer, 0);
}

void updateSliderFromMouse(int x) {
    DT = clampDouble(dtFromSliderX(x), sliderMinDt, sliderMaxDt);
    glutPostRedisplay();
}

void handleUiMouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        int bottomY = windowHeight - y;
        int panelX = hudMargin;
        int panelY = hudMargin;
        int trackX = panelX + speedTrackX;
        int trackY = panelY + speedTrackY;

        if (state == GLUT_DOWN) {
            if (pointInRect(x, bottomY, panelX, panelY, speedPanelWidth, speedPanelHeight)) {
                sliderDragging = true;
                updateSliderFromMouse(x);
                return;
            }
        } else if (state == GLUT_UP) {
            sliderDragging = false;
            return;
        }

        if (sliderDragging && state == GLUT_DOWN) {
            updateSliderFromMouse(x);
            return;
        }

        if (pointInRect(x, bottomY, trackX - 8, trackY - 12, speedTrackWidth + 16, speedTrackHeight + 32) && state == GLUT_DOWN) {
            sliderDragging = true;
            updateSliderFromMouse(x);
            return;
        }
    }

    handleCameraMouse(button, state, x, y);
}

void handleUiMotion(int x, int y) {
    if (sliderDragging) {
        updateSliderFromMouse(x);
    }
}
}

int main(int argc, char** argv) {
    initBodies();
    initOrbitRadii();
    initCamera();

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("SolarSystem");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutKeyboardFunc(handleCameraKeyboard);
    glutSpecialFunc(handleCameraSpecial);
    glutMouseFunc(handleUiMouse);
    glutMotionFunc(handleUiMotion);
    glutPassiveMotionFunc(handleUiMotion);

    glutTimerFunc(0, timer, 0);
    glutMainLoop();

    return 0;
}