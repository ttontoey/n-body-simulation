#include "BaseSimMethod.h"
#include "Particle.h"
#include <glm/glm.hpp>
#include <vector>
#include <set>
#include <iostream>


BaseSimMethod::BaseSimMethod(float G, float DT): 
    G(G), DT(DT) {}

void BaseSimMethod::addParticle(Particle p) {
    particles.push_back(p);
}

Particle BaseSimMethod::mergeParticle(int idxSrc, int idxRemove){
    float totalMass = particles[idxSrc].getMass() + particles[idxRemove].getMass();
    Particle mergedP = Particle(particles[idxSrc].getPosition(), totalMass);
    glm::vec2 newV = (particles[idxSrc].getMass() * particles[idxSrc].getVelocity() + particles[idxRemove].getMass() * particles[idxRemove].getVelocity()) / totalMass;
    glm::vec2 newA = (particles[idxSrc].getMass() * particles[idxSrc].getAcceleration() + particles[idxRemove].getMass() * particles[idxRemove].getAcceleration()) / totalMass;
    mergedP.setVelocity(newV);
    mergedP.setVelocity(newA);
    return mergedP;
}

void BaseSimMethod::mergeParticles(std::vector<std::pair<int,int>> mergeList) {
    std::vector<Particle> newParticles;
    std::set<int> mergedParticles;
    for (auto &pair : mergeList){
        mergedParticles.insert(pair.first);
        mergedParticles.insert(pair.second);
        Particle mergedP = mergeParticle(pair.first, pair.second);   
        newParticles.push_back(mergedP);     
    }
    for (int i=0 ; i < particles.size() ; i++){
        if (mergedParticles.find(i) == mergedParticles.end()){
            newParticles.push_back(particles[i]);     
        }
    }

    particles = newParticles;

}

void BaseSimMethod::collisionHandler(){

    int NUM_PARTICLES = particles.size();
    std::vector<std::pair<int,int>> toMerge;
    std::set<int> merged;

    for (int i = 0; i < NUM_PARTICLES; i++) {
        if (merged.count(i)) continue;
        for (int j = i + 1; j < NUM_PARTICLES; j++) {
            if (merged.count(j)) continue;
            glm::vec2 distVec = particles[j].getPosition() - particles[i].getPosition();
            float length = glm::length(distVec);
            if (length < 0.02f) {
                toMerge.push_back({i, j});
                merged.insert(i); merged.insert(j);
            }
        }
    }
    if (toMerge.size() > 0) mergeParticles(toMerge);
}

void BaseSimMethod::simulate(){}

void BaseSimMethod::initialize(int numParticle){}

std::vector<Particle> BaseSimMethod::getParticles(){
    return particles;
}


