// CS 3GC3 Final Project
//
// ParticleList.cpp
// -holds a list of snow or lava particles
// -and has methods to move them and draw them

#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <GLUT/glut.h>
#else
#ifdef _WIN32
# include <windows.h>
#endif
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/freeglut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include "ParticleList.h"

/**************************************************
 *      FUNCTION DECLARATIONS
 **************************************************/
float randomFloat(float,float);

/**************************************************
 *       Constructor
 **************************************************/
ParticleList::ParticleList(ParticleType type, Terrain* terrain) {

    //set globals
    particleType = type;
    terrainMap = terrain;
    particleIterator = particleList.end();

    //set bounds to entire terrain
    particleBounds[0] = -128;
    particleBounds[1] = 128;
    particleBounds[2] = 0;
    particleBounds[3] = 50;
    particleBounds[4] = -128;
    particleBounds[5] = 128;
}

/****************************************
 * clears all particles
 ****************************************/
void ParticleList::clearParticles() {
    particleList.clear();
}

/****************************************
 * moves particles and affects terrain
 * when they hit it
 ****************************************/
void ParticleList::updateParticles() {

    float newX = 0, newY = 0, newZ = 0;
    float newDirX = 0, newDirY = 0, newDirZ = 0;

    for (particleIterator = particleList.begin(); particleIterator != particleList.end(); particleIterator++) {

        if (particleIterator->age < ageLimit) {

            //calculate new position
            newX = particleIterator->position[0] + particleIterator->direction[0] * particleIterator->speed;
            newY = particleIterator->position[1] + particleIterator->direction[1] * particleIterator->speed;
            newZ = particleIterator->position[2] + particleIterator->direction[2] * particleIterator->speed;

            //affect terrain if particle hits
            if (newY <= terrainMap->getHeight(newX, newZ)) {

                particleIterator->touchedTerrain = true;
                newY = terrainMap->getHeight(newX, newZ);

                //delete snow when it hits
                if (particleType == SNOW) {
                    terrainMap->snowTerrain(newX, newZ);
                    particleIterator = particleList.erase(particleIterator);
                    particleIterator--;
                    continue;
                }
                else if (particleType == LAVA){
                    if (terrainMap->getHeight(newX,newZ) <= 3){
                        particleIterator->touchedWater = true;
                        particleIterator->setDirection(0,3,0);
                        newY = particleIterator->position[1] + particleIterator->direction[1] * particleIterator->speed;
                    }
                    else if (particleIterator->touchedWater == false)
                        terrainMap->burnTerrain(newX, newZ);
                }
            }


            //change direction vector if hasn't hit terrain
            else if (newY > terrainMap->getHeight(newX, newZ)) {

                newDirX = particleIterator->direction[0];

                if (particleType == SNOW)
                    newDirY = particleIterator->direction[1] - 0.01;
                else if (particleType == LAVA){

                    if (particleIterator->touchedWater == false)
                        newDirY = particleIterator->direction[1] - 0.08;
                    else if (particleIterator->touchedWater == true)
                        newDirY = particleIterator->direction[1];
                }


                newDirZ = particleIterator->direction[2];

                particleIterator->setDirection(newDirX, newDirY, newDirZ);
            }


            //set its new position and age
            particleIterator->setPosition(newX, newY, newZ);
            particleIterator->age = particleIterator->age + 1;
        }

        //delete particle that's too old
        else {
            particleIterator = particleList.erase(particleIterator);
            particleIterator--;
        }
    }
}

/****************************************
 * adds a particle to the particle list
 * of type of Particle List
 ****************************************/
void ParticleList::addParticle() {

    Particle p;

    if (particleType == SNOW) {

        p.setPosition(randomFloat(particleBounds[0],particleBounds[1]), 50, randomFloat(particleBounds[4],particleBounds[5]));
        p.setDirection(0, -0.1, 0);
        p.setRotation(0, 0, 0);
        p.setColour(1, 1, 1);
        p.size = 0.25;
        p.speed = 0.5;

        particleList.push_back(p);
    }

    else if (particleType == LAVA) {

        p.setPosition(terrainMap->volcanoPos[0], terrainMap->volcanoPos[1], terrainMap->volcanoPos[2]);
        p.setDirection(randomFloat(-0.25, 0.25), 3, randomFloat(-0.25, 0.25));
        p.setRotation(0,0,0);
        p.setColour(1,0,0);
        p.size = randomFloat(0.2,2.5);
        p.speed = 0.75;

        particleList.push_back(p);
    }
}

/****************************************
 * draws all particles in list
 ****************************************/
void ParticleList::drawAndAddParticles() {

    //set colour of particles
    if (particleType == SNOW) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, snowMaterial);
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, snowMaterial);
    }

    //transform particle, and draw it
    for (particleIterator = particleList.begin(); particleIterator != particleList.end(); particleIterator++) {
        glPushMatrix();

        glTranslatef(particleIterator->position[0], particleIterator->position[1], particleIterator->position[2]);
        glRotatef(particleIterator->rotation[0], 1, 0, 0);
        glRotatef(particleIterator->rotation[1], 0, 1, 0);
        glRotatef(particleIterator->rotation[2], 0, 0, 1);

        if (particleType == LAVA){
            if (particleIterator->touchedWater == true) {
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, steamMaterial);
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, steamMaterial);
                for (int i = 0;i<10;i++){
                    glTranslatef(randomFloat(0,0.5),randomFloat(0,0.5),randomFloat(0,0.5));
                    glutSolidSphere(particleIterator->size/2, 8, 4);
                }
            }
            else {
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, fireMaterial);
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, fireMaterial);
                glutSolidSphere(particleIterator->size, 8, 4);
            }
        }



        glPopMatrix();
    }

    //only adds lava particles every 8 calls
    if (enabled && !paused) {
        if (particleType == SNOW)
            addParticle();
        else if (particleType == LAVA )
            addParticle();
    }
}

void ParticleList::printStatus() {
    printf("\n\nType: %s",particleType == SNOW ? "SNOW" : "LAVA");
    printf("\nPaused: %s",paused ? "YES" : "NO");
    printf("\nEnabled: %s",enabled ? "YES" : "NO");
    printf("\nNumParticles: %lul", particleList.size());
}

/****************************************
 * returns a random float between a and br
 ****************************************/
float randomFloat(float a, float b) {
    float random = (float) rand() / RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}
