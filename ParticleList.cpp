#include "ParticleList.h"




float RandomFloatt(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}
/**************************************************
*Creates a list of particles of type Snow(0),
*Lava(1), or Steam(2)
**************************************************/
ParticleList::ParticleList(int particleType, int numberOfParticles,float particleBounds[6])
{
    particleIterator = particleList.end();

    if(particleType == 0){
        for(int i = 0;i<numberOfParticles;i++){
            particleList.push_back(Particle(RandomFloatt(particleBounds[0],particleBounds[1]),50,RandomFloatt(particleBounds[4],particleBounds[5]) ) );
            particleIterator++;
            particleIterator->setParticleDirection(RandomFloatt(0,10),-1,RandomFloatt(0,1));
            particleIterator->setParticleSize(.25);
            particleIterator->setParticleSpeed(.5);
            particleIterator->setParticleColor(1,1,1);
            particleIterator->setParticleRotAngle(0,0,0);


           // glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, 1,1,1);
            //glMaterialf(GL_FRONT_AND_BACK, GL_AMBIENT, 1,1,1);
        }
    }
    else if(particleType == 1){
        for(int i = 0;i<numberOfParticles;i++){
            particleList.push_back(Particle(-5,20,0));
            particleIterator++;
            particleIterator->setParticleDirection(-50,2,RandomFloatt(0,30));
            particleIterator->setParticleSize(.85);
            particleIterator->setParticleSpeed(.75);
            particleIterator->setParticleColor(1,0,0);
            particleIterator->setParticleRotAngle(0,0,0);

            float fire[] = {1,0,0};
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, fire);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, fire);
        }
    }
    else if(particleType == 2){
        for(int i = 0;i<numberOfParticles;i++){
            particleList.push_back(Particle(RandomFloatt(particleBounds[0],particleBounds[1]),50,RandomFloatt(particleBounds[4],particleBounds[5]) ) );
            particleIterator++;
            particleIterator->setParticleDirection(RandomFloatt(0,1),RandomFloatt(0,1),RandomFloatt(0,1));
            particleIterator->setParticleSize(.25);
            particleIterator->setParticleSpeed(.5);
            particleIterator->setParticleColor(0.5,0.5,0.5);
            particleIterator->setParticleRotAngle(0,0,0);

            //glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, 0.5,0.5,0.5);
            //glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, 0.5,0.5,0.5);
        }
    }
}

ParticleList::~ParticleList()
{
    particleList.clear();

}
void ParticleList::UpdateParticles()
{
    for(particleIterator = particleList.begin();particleIterator != particleList.end();particleIterator++){
            if (particleIterator->getParticleAge() < ageLimit){

                newX = particleIterator->getParticlePosition()[0] + .1;
                newY = particleIterator->getParticlePosition()[1]+particleIterator->getParticleDirection()[1]*particleIterator->getParticleSpeed();
                newZ = particleIterator->getParticlePosition()[2];

                if(newY <= terrain->getHeight(newX, newZ) ){

                    particleIterator->touchedTerrain = true;
                    newY = terrain->getHeight(newX, newZ);
                }

                else if(newY > terrain->getHeight(newX, newZ)){

                    newDirX = particleIterator->getParticleDirection()[0];
                    newDirY = particleIterator->getParticleDirection()[1] - .01;
                    newDirZ = particleIterator->getParticleDirection()[2];
                    particleIterator->setParticleDirection(newDirX, newDirY, newDirZ);
                }


                particleIterator->setParticlePosition(newX, newY, newZ);
                particleIterator->setParticleAge(particleIterator->getParticleAge()+1);

            }
            else {
                //particleIterator = particleList.erase(particleIterator);
                //particleIterator--;
                particleIterator->setParticleDirection(0,.5,RandomFloatt(0,1));
                particleIterator->setParticleSize(.25);
                particleIterator->setParticleSpeed(.15);
                particleIterator->setParticleColor(1,0,0);
                particleIterator->setParticleRotAngle(0,0,0);
            }
    }
}
void ParticleList::addParticle()
{
    Particle oneParticle(-50,20,0);
    oneParticle.setParticleDirection(0,.5,RandomFloatt(0,1));
    oneParticle.setParticleSize(.85);
    oneParticle.setParticleSpeed(.5);
    oneParticle.setParticleAge(0);
    oneParticle.setParticleRotAngle(0,0,0);
    particleList.push_back(oneParticle);
}

void ParticleList::DrawParticles()
{
    for(particleIterator = particleList.begin();particleIterator != particleList.end();particleIterator++){
        glPushMatrix();
            glTranslatef(particleIterator->getParticlePosition()[0], particleIterator->getParticlePosition()[1], particleIterator->getParticlePosition()[2]);
            //glTranslatef((*particleIterator).getParticlePosition()[0], (*particleIterator).getParticlePosition()[1], (*particleIterator).getParticlePosition()[2])
            glRotatef(particleIterator->getParticleRotAngle()[0], 1, 0, 0);
            glRotatef(particleIterator->getParticleRotAngle()[1], 0, 1, 0);
            glRotatef(particleIterator->getParticleRotAngle()[2], 0, 0, 1);
            glScalef(particleIterator->getParticleSize(), particleIterator->getParticleSize(), particleIterator->getParticleSize());

            glutSolidSphere(1,8,4);

        glPopMatrix();

    }
    particlesDrawn++;
    //controls flow speed of particles drawn.
    if(particlesDrawn == 16){
        addParticle();
        particlesDrawn = 0;
    }
}
