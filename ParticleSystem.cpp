/*
 CS 3GC3 Final Assignment
 Stuart Douglas - 1214422

 November 6th, 2014
 
 ParticleSystem.cpp
 -generates particles that shoot out of the emitter position
 -particles bounce off y=0 plane, losing speed and exploding
 after a set amount of time
 */

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
 *    GLOBAL VARIABLES
 ****************************************/
std::vector<ParticleSystem::Particle> particles;
std::vector<ParticleSystem::Particle> explosionParticles;

const int MAX_AGE = 300;

Terrain terrain1;

/*******************************************
* initializes values (constructor)
*******************************************/
ParticleSystem::ParticleSystem(Terrain t) {

    //initializing public global variables
    emitterPos[0] = 0;
    emitterPos[1] = 0;
    emitterPos[2] = 0;
    
    friction = true;
    spawnPaused = false;
    explosionsEnabled = true;
    
    spawnRate = 0.1;
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
        float diffuseColour[3] = {particles[i].red, particles[i].green, particles[i].blue};
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseColour);
        
        glPushMatrix();
        
        //move and rotate particle to correct position
        glTranslatef(particles[i].x, particles[i].y, particles[i].z);
        glRotatef(particles[i].xRot, 1, 0, 0);
        glRotatef(particles[i].yRot, 0, 1, 0);
        glRotatef(particles[i].zRot, 0, 0, 1);
        
        glutSolidCube(particleSize*particles[i].sizeMultiplier);
    
        glPopMatrix();
    }
}

/********************************************
 * adds particle with randomized attributes
 *******************************************/
void ParticleSystem::addParticle() {
    
    ParticleSystem::Particle particle;
    
    //randomize particle colour
    particle.red = ((double) rand() / (RAND_MAX))*0.5 + 0.5;
    particle.green = ((double) rand() / (RAND_MAX))*0.3 + 0.1;
    particle.blue = ((double) rand() / (RAND_MAX))*0.1 + 0.1;
    
    //start particle at position of emitter
    particle.x = emitterPos[0];
    particle.y = emitterPos[1];
    particle.z = emitterPos[2];
    
    //randomize x and z directions between -0.5 and 0.5
    particle.xDir = ((double) rand()/(RAND_MAX)) - 0.1;
    particle.yDir = 8;
    particle.zDir = ((double) rand()/(RAND_MAX)) - 0.1;

    //randomize rotation between 0 and 3 degrees
    particle.xRotIncr = ((double) rand()/RAND_MAX)*3.0;
    particle.yRotIncr = ((double) rand()/RAND_MAX)*3.0;
    particle.zRotIncr = ((double) rand()/RAND_MAX)*3.0;
    
    particle.sizeMultiplier = ((double) rand()/RAND_MAX)*2+0.3;
    
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
    float r = (double) rand() / RAND_MAX;
    if (!spawnPaused && r < spawnRate)
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
        particles[i].xRot = particles[i].xRot+particles[i].xRotIncr < 360 ? particles[i].xRot + particles[i].xRotIncr : 0;
        particles[i].yRot = particles[i].yRot+particles[i].yRotIncr < 360 ? particles[i].yRot + particles[i].yRotIncr : 0;
        particles[i].zRot = particles[i].zRot+particles[i].zRotIncr < 360 ? particles[i].zRot + particles[i].zRotIncr : 0;
        
        //increase age
        particles[i].age++;
        
        //remove particle below plane
        if (particles[i].y < terrain1.getHeight(particles[i].x, particles[i].z) - 1) {
            particles[i] = particles[particles.size()-1];
            particles.pop_back();
        }

        //explode particles that die of old age
        if (particles[i].age >= MAX_AGE) {
            
            float explosionOrigin[3] = {particles[i].x, particles[i].y, particles[i].z};
            float explosionColour[3] = {particles[i].red, particles[i].green, particles[i].blue};
            particles[i] = particles[particles.size()-1];
            particles.pop_back();
            
            if (!particles[i].explosion && explosionsEnabled) {

                //add some particles
                for (int i = 0; i < 15; i++) {
                
                    addParticle();
                    
                    //set origin to coordinates of particle that died
                    particles[particles.size()-1].x = explosionOrigin[0];
                    particles[particles.size()-1].y = explosionOrigin[1];
                    particles[particles.size()-1].z = explosionOrigin[2];
                    
                    //make them explode with lots of power
                    particles[particles.size()-1].xDir = (((double) rand()/(RAND_MAX)) - 0.5)*4;
                    particles[particles.size()-1].yDir = (((double) rand()/(RAND_MAX)) - 0.5)*4;
                    particles[particles.size()-1].zDir = (((double) rand()/(RAND_MAX)) - 0.5)*4;
                    
                    //set colour to similar to particle that died (but a little randomized)
                    //NOTE: this may result in a colour value < 0 or > 1, this doesn't really matter though
                    particles[particles.size()-1].red = explosionColour[0] + 0.2*(((double) rand()/RAND_MAX) - 0.5);
                    particles[particles.size()-1].green = explosionColour[1] + 0.2*(((double) rand()/RAND_MAX) - 0.5);
                    particles[particles.size()-1].blue = explosionColour[2] + 0.2*(((double) rand()/RAND_MAX) - 0.5);
                    
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
        else if (particles[i].y <= terrain1.getHeight(particles[i].x, particles[i].z)) {
            
            //bounce back up, slower and less high (if friction enabled)
            particles[i].y = terrain1.getHeight(particles[i].x, particles[i].z);
            particles[i].yDir = fabs(particles[i].yDir) * (friction ? 0.9 : 1);
            particles[i].speed = particles[i].speed * (friction? 0.7 : 1);
            
            //slowly stop rotating (so it doesn't rotate when still)
            particles[i].xRotIncr *= (friction ? 0.5 : 1);
            particles[i].yRotIncr *= (friction ? 0.5 : 1);
            particles[i].zRotIncr *= (friction ? 0.5 : 1);
        }
    }
}