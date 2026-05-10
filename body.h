#include <string>

#ifndef BODY_H
#define BODY_H

struct body {
    std::string name;
    double radius; // for GUI only
    double mass; // in solar masses
    double pos[3]; // x,y,z in AU
    double vel[3]; // x,y,z in AU/day
    double acc[3]; // x,y,z in AU/day^2
};

extern int NUM_BODIES;
extern body* bodies;

void initBodies();
void create_bodies();

#endif