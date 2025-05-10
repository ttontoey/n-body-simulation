#include "BaseSimMethod.h"
#include "Particle.h"
#include <glm/glm.hpp>
#include <vector>
#include <iostream>


BaseSimMethod::BaseSimMethod(float G, float DT): 
    G(G), DT(DT) {}

void BaseSimMethod::addParticle(Particle p) {
    particles.push_back(p);
}

void BaseSimMethod::simulate(){
    std::cout << 1 << std::endl;
}

std::vector<Particle> BaseSimMethod::getParticles(){
    return particles;
}


