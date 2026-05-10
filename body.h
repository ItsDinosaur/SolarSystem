#include <string>

#ifndef BODY_H
#define BODY_H

struct body {
    std::string name;
    double radius; // actual radius in km
    double mass; // in solar masses
    double pos[3]; // x,y,z in AU
    double vel[3]; // x,y,z in AU/day
    double acc[3]; // x,y,z in AU/day^2
};

extern double G;
extern double DT;
extern double VISUAL_SCALE;
extern double camera_eye[3];
extern double camera_center[3];
extern double camera_up[3];
extern int NUM_BODIES;
extern body* bodies;

void initBodies(const std::string& configPath = "solar_system.cfg");

#endif