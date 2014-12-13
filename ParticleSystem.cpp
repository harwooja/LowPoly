// CS 3GC3 Final Project
// 
// ParticleSystem.cpp
// -generates particles that shoot out of the emitter position
// -particles bounce off y=0 plane, losing speed and exploding
// after a set amount of time

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#ifdef _WIN32
#  include <windows.h>
#endif
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include "Terrain.h"
#include "ParticleSystem.h"
#include <vector>
#include <stdlib.h>
#include <math.h>

/*****************************************
 *    FUNCTION DECLARATIONS
 ****************************************/
float dotProduct(float* a, float* b);
float* multVectorByScalar(float s, float* a);
float* subtractVectors(float* a, float* b);

/*****************************************
 *    GLOBAL VARIABLES
 ****************************************/
std::vector<ParticleSystem::Particle> particles;
std::vector<ParticleSystem::Particle> explosionParticles;

const int MAX_AGE = 300;
Terrain* terrain1;

/*******************************************
* initializes values (constructor)
*******************************************/
ParticleSystem::ParticleSystem(Terrain* t) {

    spawnRate = 0.25;
    particleSize = 1;

    terrain1 = t;
}

/*******************************************
* iterates over all particles, drawing them
* in appropriate location
*******************************************/
void ParticleSystem::drawParticles() {

    glMatrixMode(GL_MODELVIEW);

    for (int i = 0; i < particles.size(); i++) {

        //set particle colour
        float colour[3] = {particles[i].red, particles[i].green, particles[i].blue};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, colour);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, colour);

        glPushMatrix();

        //move and rotate particle to correct position
        glTranslatef(particles[i].x, particles[i].y, particles[i].z);
        glRotatef(particles[i].xRot, 1, 0, 0);
        glRotatef(particles[i].yRot, 0, 1, 0);
        glRotatef(particles[i].zRot, 0, 0, 1);
        
        if (shape == CUBE)
            glutSolidCube(particleSize*particles[i].sizeMultiplier);
        else
            glutSolidSphere(particleSize*particles[i].sizeMultiplier, 16, 16);

        glPopMatrix();
    }
}

/********************************************
 * adds particle with randomized attributes
 *******************************************/
void ParticleSystem::addParticle() {

    ParticleSystem::Particle particle;

    //particle is reddy orangey
    particle.red = ((double) rand() / (RAND_MAX))*0.25 + 0.75;
    particle.green = ((double) rand() / (RAND_MAX))*0.5;
    particle.blue = 0;

    //start particle at position of emitter
    particle.x = terrain1->volcanoPos[0];
    particle.y = terrain1->volcanoPos[1];
    particle.z = terrain1->volcanoPos[2];
    
    //randomize x and z directions
    particle.xDir = ((double) rand()/(RAND_MAX))*0.6 - 0.3;
    particle.yDir = 6;
    particle.zDir = ((double) rand()/(RAND_MAX))*0.6 - 0.3;
    float dirVectorLength = sqrt(particle.xDir*particle.xDir + particle.yDir*particle.yDir + particle.zDir*particle.zDir);
    particle.xDir /= (dirVectorLength*0.2);
    particle.yDir /= (dirVectorLength*0.2);
    particle.zDir /= (dirVectorLength*0.2);

    //randomize rotation
    particle.xRotIncr = ((double) rand()/RAND_MAX)*4.0;
    particle.yRotIncr = ((double) rand()/RAND_MAX)*4.0;
    particle.zRotIncr = ((double) rand()/RAND_MAX)*4.0;

    //make size somewhat random
    particle.sizeMultiplier = ((double) rand()/RAND_MAX)*2+0.1;
    particle.speed = 0.2;

    //set only half of particles to explode (performance)
    particle.explosion = ((double) rand()/RAND_MAX > 0.5) ? true : false;

    particles.push_back(particle);
}

/*******************************************
* deletes all particles from system
*******************************************/
void ParticleSystem::clearParticles() {
    particles.clear();
}

/********************************************
* calculates new particle positions, including
* handling for bounces off plane
*******************************************/
void ParticleSystem::moveParticles() {

    //add a new particle
    if ((double)rand()/RAND_MAX < spawnRate)
        addParticle();

    //move particles according to their direction vectors
    for (int i = 0; i < particles.size(); i++) {

        //gravity
        particles[i].yDir -= 0.1;

        //move particle
        particles[i].x += particles[i].xDir * particles[i].speed;
        particles[i].y += particles[i].yDir * particles[i].speed;
        particles[i].z += particles[i].zDir * particles[i].speed;

        //rotate particle
        particles[i].xRot += particles[i].xRotIncr;
        particles[i].yRot += particles[i].yRotIncr;
        particles[i].zRot += particles[i].zRotIncr;

        //increase age
        particles[i].age++;

        //explode particles that die of old age
        if (particles[i].age >= MAX_AGE) {

            //remove particle
            particles[i] = particles[particles.size()-1];
            particles.pop_back();

            if (!particles[i].explosion) {

                float explosionOrigin[3] = {particles[i].x, particles[i].y, particles[i].z};

                //add some particles
                for (int i = 0; i < 5; i++) {

                    addParticle();

                    //set origin to coordinates of particle that died
                    particles[particles.size()-1].x = explosionOrigin[0];
                    particles[particles.size()-1].y = explosionOrigin[1];
                    particles[particles.size()-1].z = explosionOrigin[2];

                    //make them explode with lots of power
                    particles[particles.size()-1].xDir = (((double) rand()/(RAND_MAX)) - 0.5)*4;
                    particles[particles.size()-1].yDir = (((double) rand()/(RAND_MAX)) - 0.5)*4;
                    particles[particles.size()-1].zDir = (((double) rand()/(RAND_MAX)) - 0.5)*4;

                    //prevents it from spawning child explosions
                    particles[particles.size()-1].explosion = true;

                    //give them a shorter life
                    particles[particles.size()-1].age = 100;

                    //make them smaller
                    particles[particles.size()-1].sizeMultiplier = 0.5;
                }
            }
        }

        //bounce particle
        else if (particles[i].y+particles[i].sizeMultiplier/2.0 <= terrain1->getHeight(particles[i].x, particles[i].z)) {

            
            //set height to terrain height (if 0, delete particle as it's hit water)
            particles[i].y = terrain1->getHeight(particles[i].x, particles[i].z)+particles[i].sizeMultiplier/2.0;
            if (particles[i].y == particles[i].sizeMultiplier/2.0) {
                particles[i] = particles[particles.size()-1];
                particles.pop_back();
            }
            else {
                float* resVec = terrain1->getNormal(particles[i].x, particles[i].y);
                
                if (resVec != NULL) {
                    
                    //calculate reflection vector. Formula taken from
                    //www.3dkingdoms.com/weekly/weekly.php?a=2
                    float dirVec[3] = {particles[i].xDir, particles[i].yDir, particles[i].zDir};
                    resVec = multVectorByScalar(-2*dotProduct(dirVec,terrain1->getNormal(particles[i].x, particles[i].y)),resVec);
                    resVec = subtractVectors(resVec, dirVec);
                    float resVecLength = sqrt(resVec[0]*resVec[0]+resVec[1]*resVec[1]+resVec[2]*resVec[2]);
                    
                    //set reflection vector
                    particles[i].xDir = resVec[0]/(resVecLength*0.2);
                    particles[i].yDir = resVec[1]/(resVecLength*0.2);
                    particles[i].zDir = resVec[2]/(resVecLength*0.2);
                    
                    terrain1->burnTerrain(particles[i].x, particles[i].z);
                }
            }

            //decrease speed
            particles[i].speed = particles[i].speed * 0.5;

            //if stopped, delete it
            if (particles[i].yDir < 0.5 || particles[i].speed < 0.05) {
                particles[i] = particles[particles.size()-1];
                particles.pop_back();

                
                //decrease speed
                particles[i].speed = particles[i].speed * 0.5;
            
                //if stopped, delete it
                if (particles[i].speed < 0.05) {
                    particles[i] = particles[particles.size()-1];
                    particles.pop_back();
                }
            }
        }
    }
}

/*******************************************
 * simple vector functions (used only for
 * reflecting particles on bounce)
 *******************************************/
float dotProduct(float* a, float* b) {
    return (a[0]*b[0] + a[1]*b[1] + a[2]*b[2]);
}
float* multVectorByScalar(float s, float* a) {
    static float res[3];
    res[0] = s*a[0];
    res[1] = s*a[1];
    res[2] = s*a[2];
    return res;
}
float* subtractVectors(float* a, float* b) {
    static float res[3];
    res[0] = a[0]-b[0];
    res[1] = a[1]-b[1];
    res[2] = a[2]-b[2];
    return res;
}
