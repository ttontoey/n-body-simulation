#include "Particle.h"
#include <glm/glm.hpp>

Particle::Particle(glm::vec2 pos, glm::vec2 velocity, float mass)
    : pos(pos), velocity(velocity), mass(mass) {}

void Particle::setPosition(glm::vec2 pos) {
    this->pos = pos;
}

glm::vec2 Particle::getPosition() const {
    return pos;
}

void Particle::setVelocity(glm::vec2 velocity) {
    this->velocity = velocity;
}

glm::vec2 Particle::getVelocity() const {
    return velocity;
}

float Particle::getMass() const {
    return mass;
}