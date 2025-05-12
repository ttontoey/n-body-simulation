#include "Particle.h"
#include <glm/glm.hpp>

Particle::Particle(glm::vec2 pos, float mass)
    : pos(pos), velocity(glm::vec2(0.0, 0.0)), acceleration(glm::vec2(0.0, 0.0)), mass(mass) {}

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

void Particle::setAcceleration(glm::vec2 a) {
    this->acceleration = a;
}

glm::vec2 Particle::getAcceleration() const {
    return acceleration;
}

void Particle::setMass(float mass) {
    this->mass = mass;
}

float Particle::getMass() const {
    return mass;
}