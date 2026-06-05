#include "physics.h"
#include "body.h"
#include <cmath>

void updatePhysics() {
    for (int i = 0; i < NUM_BODIES; ++i) {
        bodies[i].acc[0] = 0.0;
        bodies[i].acc[1] = 0.0;
        bodies[i].acc[2] = 0.0;
    }

    // 2. Liczymy oddziaływania grawitacyjne (każdy z każdym)
    for (int i = 0; i < NUM_BODIES; ++i) {
        for (int j = i + 1; j < NUM_BODIES; ++j) {
            // Wektor od ciała 'i' do ciała 'j'
            double dx = bodies[j].pos[0] - bodies[i].pos[0];
            double dy = bodies[j].pos[1] - bodies[i].pos[1];
            double dz = bodies[j].pos[2] - bodies[i].pos[2];


            double distSq = dx*dx + dy*dy + dz*dz;
            

            if (distSq < 1e-10) continue; 
            
            double dist = std::sqrt(distSq);

            // Obliczamy wartość siły dzieloną przez masę (czyli samo G / r^3)
            // Z prawa powszechnego ciążenia Newtona: a = G * M / r^2
            // Dzielimy dodatkowo przez dist, bo mnożymy potem przez wektory dx, dy, dz
            double forceMag = G / (distSq * dist);

            double ax = forceMag * dx;
            double ay = forceMag * dy;
            double az = forceMag * dz;

            // Dodajemy przyspieszenie do obu ciał (III zasada dynamiki Newtona)
            // Ciało 'i' jest przyciągane przez masę ciała 'j'
            bodies[i].acc[0] += ax * bodies[j].mass;
            bodies[i].acc[1] += ay * bodies[j].mass;
            bodies[i].acc[2] += az * bodies[j].mass;

            // Ciało 'j' jest przyciągane przez masę ciała 'i'
            bodies[j].acc[0] -= ax * bodies[i].mass;
            bodies[j].acc[1] -= ay * bodies[i].mass;
            bodies[j].acc[2] -= az * bodies[i].mass;
        }
    }

    // Euer
    for (int i = 0; i < NUM_BODIES; ++i) {
        // A) Najpierw aktualizujemy prędkość
        bodies[i].vel[0] += bodies[i].acc[0] * DT;
        bodies[i].vel[1] += bodies[i].acc[1] * DT;
        bodies[i].vel[2] += bodies[i].acc[2] * DT;

        bodies[i].pos[0] += bodies[i].vel[0] * DT;
        bodies[i].pos[1] += bodies[i].vel[1] * DT;
        bodies[i].pos[2] += bodies[i].vel[2] * DT;
    }
}