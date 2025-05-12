#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/glm.hpp>

class Particle {
    public:
        Particle(glm::vec2 pos, float mass);
        void setPosition(glm::vec2 pos);
        glm::vec2 getPosition() const;
        void setVelocity(glm::vec2 velocity);
        glm::vec2 getVelocity() const;
        void setAcceleration(glm::vec2 a);
        glm::vec2 getAcceleration() const;
        void setMass(float mass);
        float getMass() const;
        

    private:
        glm::vec2 pos;
        glm::vec2 velocity;
        glm::vec2 acceleration;
        float mass;
};

#endif