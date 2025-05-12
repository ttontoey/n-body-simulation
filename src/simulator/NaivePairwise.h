#ifndef NAIVEPAIRWISE_H
#define NAIVEPAIRWISE_H

#include "./BaseSimMethod.h"

class NaivePairwise : public BaseSimMethod {
public:
    NaivePairwise(float G, float DT);
    void simulate() override;
    void initialize(int numParticle) override;
    std::vector<Particle> getParticles() override;
};

#endif