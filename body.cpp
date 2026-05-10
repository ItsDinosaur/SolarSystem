#include "body.h"
#include <fstream>
#include <limits>
#include <vector>
using namespace std;

double G = 0.0002959122082855911;
double DT = 0.5;
double VISUAL_SCALE = 1e-5;
double camera_eye[3] = {30.0, 40.0, 50.0};
double camera_center[3] = {0.0, 0.0, 0.0};
double camera_up[3] = {0.0, 1.0, 0.0};


int NUM_BODIES = 0;
body* bodies = nullptr;

namespace {
void clearBodies() {
    delete[] bodies;
    bodies = nullptr;
    NUM_BODIES = 0;
}

void setBodies(const vector<body>& source) {
    clearBodies();
    NUM_BODIES = static_cast<int>(source.size());
    bodies = new body[NUM_BODIES];
    for (int i = 0; i < NUM_BODIES; ++i) {
        bodies[i] = source[i];
    }
}

void loadDefaultBodies() {
    vector<body> defaults;
    defaults.push_back({"Sun", 696340.0, 1.0, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}});
    defaults.push_back({"Mercury", 2439.7, 1.659e-7, {0.38709927, 0, 0}, {0, 47.36, 0}, {0, 0, 0}});
    defaults.push_back({"Venus", 6051.8, 2.447e-6, {0.72333566, 0, 0}, {0, 35.02, 0}, {0, 0, 0}});
    defaults.push_back({"Earth", 6371.0, 3.003e-6, {1.0, 0, 0}, {0, 29.78, 0}, {0, 0, 0}});
    defaults.push_back({"Mars", 3389.5, 3.213e-7, {1.52371034, 0, 0}, {0, 24.077, 0}, {0, 0, 0}});
    defaults.push_back({"Jupiter", 69911.0, 9.545e-4, {5.20288700, 0, 0}, {0, 13.07, 0}, {0, 0, 0}});
    defaults.push_back({"Saturn", 58232.0, 2.858e-4, {9.53667594, 0, 0}, {0, 9.69, 0}, {0, 0, 0}});
    defaults.push_back({"Uranus", 25362.0, 4.366e-5, {-19.18916464, 0, 0}, {0, 6.81, 0}, {0, 0, 0}});
    defaults.push_back({"Neptune", 24622.0, 5.151e-5, {30.06992276, 0, 0}, {0, 5.43, 0}, {0, 0, 0}});
    setBodies(defaults);
}

bool loadBodiesFromFile(const string& configPath) {
    ifstream input(configPath.c_str());
    if (!input) {
        return false;
    }

    vector<body> loadedBodies;
    string token;

    while (input >> token) {
        if (token.empty() || token[0] == '#') {
            input.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (token == "gravity") {
            input >> G;
        } else if (token == "dt") {
            input >> DT;
        } else if (token == "visual_scale") {
            input >> VISUAL_SCALE;
        } else if (token == "camera_eye") {
            input >> camera_eye[0] >> camera_eye[1] >> camera_eye[2];
        } else if (token == "camera_center") {
            input >> camera_center[0] >> camera_center[1] >> camera_center[2];
        } else if (token == "camera_up") {
            input >> camera_up[0] >> camera_up[1] >> camera_up[2];
        } else if (token == "body") {
            body current{};
            if (!(input >> current.name >> current.radius >> current.mass
                  >> current.pos[0] >> current.pos[1] >> current.pos[2]
                  >> current.vel[0] >> current.vel[1] >> current.vel[2])) {
                return false;
            }
            current.acc[0] = 0.0;
            current.acc[1] = 0.0;
            current.acc[2] = 0.0;
            loadedBodies.push_back(current);
        } else {
            input.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    if (loadedBodies.empty()) {
        return false;
    }

    setBodies(loadedBodies);
    return true;
}
}

void initBodies(const string& configPath) {
    if (!loadBodiesFromFile(configPath)) {
        loadDefaultBodies();
    }
}