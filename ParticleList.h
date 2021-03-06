// CS 3GC3 Final Project
//
// ParticleList.h
// -public interface for particle list

#ifndef __TERRAIN__
#define __TERRAIN__
#include "Terrain.h"
#endif

#include <list>

class ParticleList {

public:

    /****************************************
     *    PUBLIC CUSTOM STRUCTS
     ****************************************/
    typedef struct Particle {

        //vector variables
        double position[3] = {0,0,0};
        void setPosition(float posX, float posY, float posZ) {
            position[0] = posX;
            position[1] = posY;
            position[2] = posZ;
        }
        float direction[3] = {0,0,0};
        void setDirection(float dirX, float dirY, float dirZ) {
            direction[0] = dirX;
            direction[1] = dirY;
            direction[2] = dirZ;
        }
        float colour[3] = {0,0,0};
        void setColour(float colR, float colG, float colB) {
            colour[0] = colR;
            colour[1] = colG;
            colour[2] = colB;
        }

        //scalar variables
        float age = 0;
        float size = 1;
        float speed = 1;

        bool touchedTerrain = false;
        bool touchedWater = false;

    } Particle;
    typedef enum {SNOW, LAVA, FOG} ParticleType;

    /****************************************
     *    PUBLIC FUNCTIONS
     ****************************************/
    ParticleList(ParticleType particleType, Terrain*);
    
    void addParticle();
    void updateParticles();
    void drawAndAddParticles();
    void clearParticles();
    bool deathCollision(float x, float y, float z);

    /****************************************
     *    PUBLIC VARIABLES
     ****************************************/
    bool enabled = true;
    bool paused = false;

private:

    /****************************************
     *    PRIVATE VARIABLES
     ****************************************/
    std::list<Particle> particleList;
    std::list<Particle>::iterator particleIterator;

    Terrain *terrainMap;
    ParticleType particleType;

    int ageLimit = 700;
    int particlesDrawn = 0;
    float particleBounds[6];
    double paddingXlow;
    double paddingXhigh;
    double paddingZlow;
    double paddingZhigh;
    
    float snowMaterial[4] = {1,1,1,1};
    float fireMaterial[4] = {1,0,0,1};
    float specMaterial[4] = {0.5,0.5,0.5,1};
    float steamMaterial[4] = {0.7,0.7,0.7,0.75};

};
