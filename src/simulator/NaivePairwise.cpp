#include "NaivePairwise.h"
#include "Particle.h"
#include <glm/glm.hpp>
#include <iostream>
#include <set>



NaivePairwise::NaivePairwise(float G, float DT, bool enableCollision)
    : BaseSimMethod(G, DT, enableCollision) {}

void NaivePairwise::initialize(int numParticle) {
    for (int i = 0; i < numParticle; i++) {
        glm::vec2 position = glm::vec2(rand() % 1000 / 500.0f - 1.0f, rand() % 1000 / 500.0f - 1.0f);
        float mass = 1.0;
        Particle p(position, mass);
        particles.push_back(p);
    }
}

void NaivePairwise::simulate() {

    // Handle inelastic collisions, if any
    collisionHandler();
    
    int NUM_PARTICLES = particles.size();
    for (int i = 0; i < NUM_PARTICLES; ++i) {
        glm::vec2 force(0.0f);
        float potentialEnergy = 0.0;
        for (int j = 0; j < NUM_PARTICLES; ++j) {
            if (i == j) continue; // Same particle, computation skipped
            glm::vec2 distVec = particles[j].getPosition() - particles[i].getPosition();
            float rawDistSqr = glm::dot(distVec, distVec);
            if (rawDistSqr < 1e-8f) continue;    // Avoid normalize(0)
            float distSqr = rawDistSqr + 1e-4f;  // Softening
            float dist = sqrt(distSqr);
            glm::vec2 direction = distVec / dist;
            float strength = (G * particles[i].getMass() * particles[j].getMass()) / distSqr;
            force += strength * direction;
        }

        particles[i].setAcceleration(force / particles[i].getMass());
        glm::vec2 newVelocity = particles[i].getVelocity() + DT * particles[i].getAcceleration();
        particles[i].setVelocity(newVelocity);
    }

    for (int i = 0; i < NUM_PARTICLES; ++i) {
        particles[i].setPosition(particles[i].getPosition() + DT * particles[i].getVelocity());
    }

}

std::vector<Particle> NaivePairwise::getParticles()  {
    return particles;
}