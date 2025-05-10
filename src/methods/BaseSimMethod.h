#ifndef BASESIMMETHOD_H
#define BASESIMMETHOD_H

#include "Particle.h"
#include <vector>
#include <glm/glm.hpp>

class BaseSimMethod {
    public:
        BaseSimMethod(float G, float DT);
        virtual void simulate();
        virtual std::vector<Particle> getParticles();
        void addParticle(Particle p);
    protected:
        std::vector<Particle> particles;
        float G;
        float DT;
};

#endif