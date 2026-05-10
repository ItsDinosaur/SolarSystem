// create all planets, initialize their positions and velocities
// all data from needed taken from NASA
#include "body.h"
#include <iostream>
using namespace std;

int NUM_BODIES = 9;
body* bodies = nullptr;

void create_bodies(){
    bodies = new body[NUM_BODIES];
    
    bodies[0] = {"Sun", 30, 1.0, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}};
    bodies[1] = {"Mercury", 0.3, 1.659e-7, {0.38709927, 0, 0}, {0, 47.36, 0}, {0, 0, 0}};
    bodies[2] = {"Venus", 0.3, 2.447e-6, {0.72333566, 0, 0}, {0, 35.02, 0}, {0, 0, 0}};
    bodies[3] = {"Earth", 1.0, 3.003e-6, {1.0, 0, 0}, {0, 29.78, 0}, {0, 0, 0}};
    bodies[4] = {"Mars", 0.8, 3.213e-7, {1.52371034, 0, 0}, {0, 24.077, 0}, {0, 0, 0}};
    bodies[5] = {"Jupiter", 11, 9.545e-4, {5.20288700, 0, 0}, {0, 13.07, 0}, {0, 0, 0}};
    bodies[6] = {"Saturn", 9, 2.858e-4, {9.53667594, 0, 0}, {0, 9.69, 0}, {0, 0, 0}};
    bodies[7] = {"Uranus", 4, 4.366e-5, {-19.18916464, 0 ,0}, {0 ,6.81 ,0}, {0 ,0 ,0}};
    bodies[8] = {"Neptune", 4 ,5.151e-5 ,{30.06992276 ,0 ,0} ,{0 ,5.43 ,0} ,{0 ,0 ,0}};

};

void initBodies() {
    create_bodies();
}