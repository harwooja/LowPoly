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

#include <list>
#include <math.h>

#include "Particle.h"
#include "Terrain.h"

using namespace std;

class ParticleList {

    public:
        ParticleList(int particleType, float particleBounds[6]);
        ~ParticleList();
        void UpdateParticles(Terrain terrainMap);
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
        //Terrain *terrain;
        int particlesDrawn = 0;
        float particleBounds[6];
        int particleType;
        float snow[3] = {1,1,1};
        float fire[3] = {1,0,0};
        float amb[3] = {0,0,0};
        float spc[3] = {0,0,0};
};
