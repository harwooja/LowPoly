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

#include <math.h>
#include <stdlib.h>

#include "Particle.h"
#include "ParticleList.h"

float RandomFloatt(float a, float b) {
    float random = (float) rand()/RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

/**************************************************
 *Creates a list of particles of type Snow(0),
 *Lava(1), or Steam(2)
 **************************************************/
ParticleList::ParticleList(int typeOfParticle, float boundsOfParticle[6], Terrain* terrain) {
    
    particleType = typeOfParticle;
    for (int i = 0; i < 6; i++) {
        particleBounds[i] = boundsOfParticle[i];
    }
    
    terrainMap = terrain;

    particleIterator = particleList.end();
    
    if (particleType == 0) {
        particleList.push_back(Particle(RandomFloatt(particleBounds[0],particleBounds[1]),50,RandomFloatt(particleBounds[4],particleBounds[5])));
        particleIterator++;
        particleIterator->setParticleDirection(RandomFloatt(0,10), -1, RandomFloatt(0,1));
        particleIterator->setParticleSize(.75);
        particleIterator->setParticleSpeed(.5);
        particleIterator->setParticleColor(1,1,1);
        particleIterator->setParticleRotAngle(0,0,0);
    }
    else if (particleType == 1) {
        particleList.push_back(Particle(terrainMap->volcanoPos[0],terrainMap->volcanoPos[1],terrainMap->volcanoPos[2]));
        particleIterator++;
        particleIterator->setParticleDirection(-50,2,RandomFloatt(0,30));
        particleIterator->setParticleSize(.85);
        particleIterator->setParticleSpeed(.75);
        particleIterator->setParticleColor(1,0,0);
        particleIterator->setParticleRotAngle(0,0,0);
    }
    else if (particleType == 2) {
        particleList.push_back(Particle(RandomFloatt(particleBounds[0],particleBounds[1]),50,RandomFloatt(particleBounds[4],particleBounds[5]) ) );
        particleIterator++;
        particleIterator->setParticleDirection(RandomFloatt(0,1),RandomFloatt(0,1),RandomFloatt(0,1));
        particleIterator->setParticleSize(.25);
        particleIterator->setParticleSpeed(.5);
        particleIterator->setParticleColor(0.5,0.5,0.5);
        particleIterator->setParticleRotAngle(0,0,0);
    }
}

ParticleList::~ParticleList() {
    particleList.clear();
}

void ParticleList::UpdateParticles() {
    
    for(particleIterator = particleList.begin();particleIterator != particleList.end();particleIterator++) {
        
        if (particleIterator->getParticleAge() < ageLimit) {
            if (particleType == 0) {
                newX = particleIterator->getParticlePosition()[0];
                newY = particleIterator->getParticlePosition()[1]+particleIterator->getParticleDirection()[1]*particleIterator->getParticleSpeed();
                newZ = particleIterator->getParticlePosition()[2];
            }
            if (particleType == 1) {
                newX = particleIterator->getParticlePosition()[0]+particleIterator->getParticleDirection()[0]*particleIterator->getParticleSpeed();
                newY = particleIterator->getParticlePosition()[1]+particleIterator->getParticleDirection()[1]*particleIterator->getParticleSpeed();
                newZ = particleIterator->getParticlePosition()[2]+particleIterator->getParticleDirection()[2]*particleIterator->getParticleSpeed();
            }
            
            {
                if (newY <= terrainMap->getHeight(newX, newZ)) {
                    particleIterator->touchedTerrain = true;
                    newY = terrainMap->getHeight(newX, newZ);
                    if (particleType == 0) {
                       terrainMap->snowTerrain(newX, newZ);
                    }
                    if (particleType == 1) {
                        terrainMap->burnTerrain(newX, newZ);
                    }
                }
                
                else if (newY > terrainMap->getHeight(newX, newZ)) {
                    newDirX = particleIterator->getParticleDirection()[0]+particleIterator->getParticleDirection()[0]*particleIterator->getParticleSpeed();
                    newDirY = particleIterator->getParticleDirection()[1] - .01;
                    newDirZ = particleIterator->getParticleDirection()[2]+particleIterator->getParticleDirection()[2]*particleIterator->getParticleSpeed();
                    particleIterator->setParticleDirection(newDirX, newDirY, newDirZ);
                }
            }
            
            particleIterator->setParticlePosition(newX, newY, newZ);
            particleIterator->setParticleAge(particleIterator->getParticleAge()+1);
        }
        else {
            particleIterator = particleList.erase(particleIterator);
            particleIterator--;
        }
    }
}

void ParticleList::addParticle() {
    
    Particle oneParticle(0,0,0);
    
    if (particleType == 0) {
        oneParticle.setParticlePosition(RandomFloatt(particleBounds[0],particleBounds[1]),50,RandomFloatt(particleBounds[4],particleBounds[5]));
        oneParticle.setParticleDirection(0,-.1,0);
        oneParticle.setParticleSize(.75);
        oneParticle.setParticleSpeed(.5);
        oneParticle.setParticleAge(0);
        oneParticle.setParticleRotAngle(0,0,0);
        particleList.push_back(oneParticle);
    }
    else if (particleType == 1) {
        oneParticle.setParticlePosition(terrainMap->volcanoPos[0],terrainMap->volcanoPos[1],terrainMap->volcanoPos[2]);
        oneParticle.setParticleDirection(RandomFloatt(-.5,.5),3,RandomFloatt(-.5,.5));
        oneParticle.setParticleSize(RandomFloatt(0,2));
        oneParticle.setParticleSpeed(.15);
        oneParticle.setParticleAge(0);
        oneParticle.setParticleRotAngle(0,0,0);
        particleList.push_back(oneParticle);
    }
}

void ParticleList::DrawParticles() {
    
    for(particleIterator = particleList.begin();particleIterator != particleList.end();particleIterator++){
        
        glPushMatrix();
        
        glTranslatef(particleIterator->getParticlePosition()[0], particleIterator->getParticlePosition()[1], particleIterator->getParticlePosition()[2]);
        glRotatef(particleIterator->getParticleRotAngle()[0], 1, 0, 0);
        glRotatef(particleIterator->getParticleRotAngle()[1], 0, 1, 0);
        glRotatef(particleIterator->getParticleRotAngle()[2], 0, 0, 1);

        if (particleType == 0) {
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, snow);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
            glutSolidSphere(particleIterator->getParticleSize(),8,4);
        }
        else if (particleType == 1) {
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, fire);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
            glutSolidSphere(particleIterator->getParticleSize(),8,4);
        }
        glPopMatrix();
    }
    particlesDrawn++;
    
    //controls flow speed of particles drawn.
    if(particlesDrawn == 16 && particleType==1){
        addParticle();
        particlesDrawn = 0;
    }
    else if (particleType == 0)
        addParticle();
}

void ParticleList::rotateParticle(float incrX, float incrY, float incrZ) {
    for(particleIterator = particleList.begin();particleIterator != particleList.end();particleIterator++) {
        particleIterator->setParticleRotAngle(particleIterator->getParticleRotAngle()[0]+incrX,
                                              particleIterator->getParticleRotAngle()[1]+incrY,
                                              particleIterator->getParticleRotAngle()[2]+incrZ);
    }
}
