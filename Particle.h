#ifndef PARTICLE_H
#define PARTICLE_H

class Particle
{
    public:
        Particle(float x, float y, float z);
        Particle();

        //getters and setters for particle attributes
        float *getParticlePosition();
        float getParticleSpeed();
        float *getParticleDirection();
        float *getParticleRotAngle();
        float getParticleSize();
        float *getParticleColor();
        float getParticleAge();

        void setParticlePosition(float px, float py, float pz);
        void setParticleSpeed(float speed);
        void setParticleDirection(float dx, float dy, float dz);
        void setParticleRotAngle(float rx, float ry, float rz);
        void setParticleSize(float s);
        void setParticleColor(float r, float g, float b);
        void setParticleAge(float age);
        bool touchedTerrain = false;

    protected:
    private:

        float particleX;
        float particleY;
        float particleZ;

        float particleDirectionX;
        float particleDirectionY;
        float particleDirectionZ;

        float rotationAngleX;
        float rotationAngleY;
        float rotationAngleZ;

        float particleSpeed;

        float particleColorR;
        float particleColorG;
        float particleColorB;

        float particleAge;
        float particleSize;



};

#endif // PARTICLE_H
