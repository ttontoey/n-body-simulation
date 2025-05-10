#include "NaivePairwise.h"
#include "Particle.h"
#include <glm/glm.hpp>
#include <iostream>


NaivePairwise::NaivePairwise(float G, float DT)
    : BaseSimMethod(G, DT) {}

void NaivePairwise::simulate() {
    int NUM_PARTICLES = particles.size();
    for (int i = 0; i < NUM_PARTICLES; ++i) {
        glm::vec2 force(0.0f);
        for (int j = 0; j < NUM_PARTICLES; ++j) {
            if (i == j) continue; // Same particle, computation skipped
            glm::vec2 distVec = particles[j].getPosition() - particles[i].getPosition();
            float distSqr = glm::dot(distVec, distVec) + 1e-4f;
            float strength = (G * particles[i].getMass() * particles[j].getMass()) / distSqr;
            force += strength * glm::normalize(distVec);
        }
        particles[i].setVelocity(particles[i].getVelocity() + DT * force / particles[i].getMass());
    }

    for (int i = 0; i < NUM_PARTICLES; ++i) {
        particles[i].setPosition(particles[i].getPosition() += DT * particles[i].getVelocity());
    }

}

std::vector<Particle> NaivePairwise::getParticles()  {
    return particles;
}