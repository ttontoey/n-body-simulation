#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>

class Particle {
    public:
        Particle(glm::vec2 pos, glm::vec2 velocity, float mass);
        void setPosition(glm::vec2 pos);
        glm::vec2 getPosition() const;
        void setVelocity(glm::vec2 velocity);
        glm::vec2 getVelocity() const;
        float getMass() const;

    private:
        glm::vec2 pos;
        glm::vec2 velocity;
        float mass;
};

#endif