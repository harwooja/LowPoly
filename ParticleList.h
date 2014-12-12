#ifndef PARTICLELIST_H
#define PARTICLELIST_H
#include "Particle.h"
#include <list>
#include <Math.h>
#include "Terrain.h"
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
using namespace std;

class ParticleList
{
    public:
        ParticleList(int particleType, int numberOfParticles, float particleBounds[6]);
        ~ParticleList();
        void UpdateParticles();
        void DrawParticles();
        void addParticle();

    private:
        std::list<Particle> particleList;
        list<Particle>::iterator particleIterator;
        int ageLimit = 500;
        float newX;
        float newY;
        float newZ;
        float newDirX;
        float newDirY;
        float newDirZ;
        Terrain *terrain;
        int particlesDrawn = 0;

};

#endif // PARTICLELIST_H
