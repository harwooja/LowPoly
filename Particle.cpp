#include "Particle.h"
#include <stdlib.h>

Particle::Particle(float x, float y, float z) {
    setParticlePosition(x,y,z);
    setParticleAge(0);
    setParticleColor(0,0,0);
    setParticleDirection(0,0,0);
    setParticleRotAngle(0,0,0);
    setParticleSize(0);
    setParticleSpeed(0);
}

Particle::Particle() {
    setParticleAge(0);
    setParticleColor(0,0,0);
    setParticleDirection(0,0,0);
    setParticlePosition(0,0,0);
    setParticleRotAngle(0,0,0);
    setParticleSize(0);
    setParticleSpeed(0);
}

//returns the particles x,y,z coordinates in the form of an array
float* Particle::getParticlePosition() {
//    float a[3] = {particleX, particleY, particleZ};
    float* a = (float*) malloc(sizeof(float)*3);
    a[0] = particleX;
    a[1] = particleY;
    a[2] = particleZ;
    return a;
}

void Particle::setParticlePosition(float px, float py, float pz) {
    particleX = px;
    particleY = py;
    particleZ = pz;
}

//returns the particles direction coordinates in the form of an array
float* Particle::getParticleDirection() {
    float* a = (float*) malloc(sizeof(float)*3);
    a[0] = particleDirectionX;
    a[1] = particleDirectionY;
    a[2] = particleDirectionZ;
    return a;
}

//sets the particles direction coordinates
void Particle::setParticleDirection(float dx, float dy, float dz) {
    particleDirectionX = dx;
    particleDirectionY = dy;
    particleDirectionZ = dz;
}

//returns for rotation angle's for each axis in the form of an array
float* Particle::getParticleRotAngle() {
    float* a = (float*) malloc(sizeof(float)*3);
    a[0] = rotationAngleX;
    a[1] = rotationAngleY;
    a[2] = rotationAngleZ;
    return a;
}

//sets the particle's rotation angles
void Particle::setParticleRotAngle(float rx, float ry, float rz) {
    rotationAngleX = rx;
    rotationAngleY = ry;
    rotationAngleZ = rz;
}

//returns the particle's speed
float Particle::getParticleSpeed() {
    return particleSpeed;
}

//sets the particle's speed
void Particle::setParticleSpeed(float speed) {
    particleSpeed = speed;
}

//returns the particle's colors (r,g,b) in the form of an array
float* Particle::getParticleColor() {
    float* a = (float*) malloc(sizeof(float)*3);
    a[0] = particleColorR;
    a[1] = particleColorG;
    a[2] = particleColorB;
    return a;
}

//sets the r, g, b of the particle
void Particle::setParticleColor(float r, float g, float b) {
    particleColorR = r;
    particleColorG = g;
    particleColorB = b;
}

//returns the particle's age
float Particle::getParticleAge() {
    return particleAge;
}

//sets the particle's age
void Particle::setParticleAge(float age) {
    particleAge = age;
}

//sets the particle's size
float Particle::getParticleSize() {
    return particleSize;
}

void Particle::setParticleSize(float s) {
    particleSize = s;
}
