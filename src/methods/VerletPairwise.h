#ifndef VERLETPAIRWISE_H
#define VERLETPAIRWISE_H

#include "./BaseSimMethod.h"

class VerletPairwise : public BaseSimMethod {
public:
    VerletPairwise(float G, float DT);
    void simulate() override;
    void initialize(int numParticle) override;
    std::vector<Particle> getParticles() override;
};

#endif