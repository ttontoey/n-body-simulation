#ifndef BASESIMMETHOD_H
#define BASESIMMETHOD_H

#include "Particle.h"
#include <vector>
#include <glm/glm.hpp>

class BaseSimMethod {
    public:
        BaseSimMethod(float G, float DT, bool enableCollision);
        virtual void simulate();
        virtual void initialize(int numParticle);
        virtual std::vector<Particle> getParticles();
        float getTotalEnergy() const;

    protected:
        std::vector<Particle> particles;
        float G;
        float DT;
        bool enableCollision;
        Particle mergeParticle(int idxSrc, int idxRemove);
        void addParticle(Particle p);
        void mergeParticles(std::vector<std::pair<int,int>> mergeList);
        void collisionHandler();
};

#endif