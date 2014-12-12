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
#include <vector>
#include <stdlib.h>
#include <math.h>
//#include "Terrain.h"
#include "Particle.h"
#include "ParticleList.h"
#include "ParticleSystem.h"
#include "Camera.h"



using namespace std;

/*****************************************
*    FUNCTION DECLARATIONS
****************************************/
void drawAxes();

/*****************************************
 *    GLOBAL VARIABLES
 ****************************************/
Terrain terrain;
//ParticleSystem volcanoParticles(&terrain);
Camera camera;
//ParticleSystem snowParticles(&terrain);

bool fullscreen = false;
bool paused = false;
float lightPos[4] = {0,60,0, 1};

bool mouseCurrentInitiated = false;
int currX = 0;
int currY = 0;
int windowWidth = 800;
int windowHeight = 600;
/***********************************
*added some more global variables
*added 3 methods to create, update and draw the particles
*added some helper methods
*snowMode, lavaMode turn snow and lava on and off
*************************************/
float ageLimit = 1000;
float friction = 0.5;
float gravity = 1.025;
//xmin,xmax,ymin,ymax,zmin,zmax
float particleBounds[] = {-50,50,4,50,-50,50};
float angle = 0;
float angle2 = 0;
float angle3 = 0;

bool snowMode = false;
bool lavaMode = false;
bool steamMode = false;

float snowColor[] = {1,1,1};
float lavaColor[] = {1,0,0};
float steamColor[] = {0.5,0.5,0.5};

ParticleList fireParticles(1,1,particleBounds);

std::list<Particle> particleList;
std::list<Particle> steamParticleList;
std::list<Particle> lavaParticleList;
list<Particle>::iterator steamParticleIterator = steamParticleList.end();
list<Particle>::iterator lavaParticleIterator = lavaParticleList.end();
list<Particle>::iterator particleIterator = particleList.end();
list<Particle>::iterator particleIterator3;
list<Particle>::iterator particleIterator2;

float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}
void drawCube(float r, float g, float b)
{
	glBegin(GL_QUADS);

	//front
	glColor3f(r, g, b);
	glVertex3f(-1, -1, 1);
	glVertex3f(1, -1, 1);
	glVertex3f(1, 1, 1);
	glVertex3f(-1, 1, 1);

	//top
	glColor3f(r, g, b);
	glVertex3f(-1,1,1);
	glVertex3f(1,1,1);
	glVertex3f(1,1,-1);
	glVertex3f(-1,1,-1);

	//bottom
	glColor3f(r, g, b);
	glVertex3f(-1,-1,1);
	glVertex3f(1,-1,1);
	glVertex3f(1,-1,-1);
	glVertex3f(-1,-1,-1);

	//left side
	glColor3f(r, g, b);
	glVertex3f(-1,1,1);
	glVertex3f(-1,-1,1);
	glVertex3f(-1,-1,-1);
	glVertex3f(-1,1,-1);

	//right side
	glColor3f(r,g,b);
	glVertex3f(1,1,1);
	glVertex3f(1,-1,1);
	glVertex3f(1,-1,-1);
	glVertex3f(1,1,-1);

	//back side
	glColor3f(r,g,b);
	glVertex3f(-1,1,-1);
	glVertex3f(-1,-1,-1);
	glVertex3f(1,-1,-1);
	glVertex3f(1,1,-1);

	glEnd();
}

/*****************************************
 * snowMode: 0
 * lavaMode: 1
 * steamMode: 2
 ****************************************/
void CreateParticles(int particleType)
{
    for (int i = 0;i<1;i++){
        if(particleType == 0){
            particleList.push_back(Particle(RandomFloat(particleBounds[0],particleBounds[1]),50,RandomFloat(particleBounds[4],particleBounds[5]) ) );
            particleIterator++;
            particleIterator->setParticleDirection(RandomFloat(0,10),-1,RandomFloat(0,1));
            particleIterator->setParticleSize(.25);
            particleIterator->setParticleSpeed(.5);
            particleIterator->setParticleColor(1,1,1);
            //particleIterator->setParticleRotAngle();

            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, snowColor);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, snowColor);

        }
        else if(particleType == 1){

            lavaParticleList.push_back(Particle(-50,60,0));
            lavaParticleIterator++;
            lavaParticleIterator->setParticleDirection(3,15,RandomFloat(0,1));
            lavaParticleIterator->setParticleSize(.25);
            lavaParticleIterator->setParticleSpeed(.025);
            lavaParticleIterator->setParticleColor(1,0,0);
            //lavaParticleIterator->setParticleRotAngle();

            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, lavaColor);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, lavaColor);

        }
        else if(particleType == 2){
                //the steam particles are generated at the same location as the collision of steam particles and steam tiles
            steamParticleList.push_back(Particle(RandomFloat(particleBounds[0],particleBounds[1]),50,RandomFloat(particleBounds[4],particleBounds[5]) ) );
            steamParticleIterator++;
            steamParticleIterator->setParticleDirection(RandomFloat(0,1),RandomFloat(0,1),RandomFloat(0,1));
            steamParticleIterator->setParticleSize(.25);
            steamParticleIterator->setParticleSpeed(.5);
            steamParticleIterator->setParticleColor(0.5,0.5,0.5);

            //steamParticleIterator->setParticleRotAngle()
            glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, steamColor);
            glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, steamColor);

        }
    }
}
/*****************************************
 * snowMode: 0
 * lavaMode: 1
 * steamMode: 2
 ****************************************/
void UpdateParticles(int particleType)
{
    if(particleType == 1){
        for(particleIterator2 = lavaParticleList.begin();particleIterator2 != lavaParticleList.end();particleIterator2++){

            //if the particle's age is less than the limit, update its values
            if (particleIterator2->getParticleAge() < ageLimit){
                float newX = particleIterator2->getParticlePosition()[0] + particleIterator2->getParticleDirection()[0]*particleIterator2->getParticleSpeed();
                float newY = particleIterator2->getParticlePosition()[1] + particleIterator2->getParticleDirection()[1]*particleIterator2->getParticleSpeed();
                float newZ = particleIterator2->getParticlePosition()[2] + particleIterator2->getParticleDirection()[2]*particleIterator2->getParticleSpeed();

                //if the particle is within the platform boundaries let it stay on the platform imitating flow
                if ((newX <=particleBounds[1] && newX >particleBounds[0]) && (newZ <=particleBounds[5] && newZ > particleBounds[4])){

                    //if the direction is positive, then moving upwards, so gravity should reduce the speed
                    if(particleIterator2->getParticleDirection()[1] > 0){
                        particleIterator2->setParticleSpeed(particleIterator2->getParticleSpeed()/gravity);

                        //if the difference is infintismely small and positive (to confirm the particle is still moving up), reverse the direction
                        if ((newY - particleIterator2->getParticlePosition()[1] < 0.05) && (newY- particleIterator2->getParticlePosition()[1] > 0)){
                            particleIterator2->setParticleDirection(particleIterator2->getParticleDirection()[0],
                                                                    (-1)*particleIterator2->getParticleDirection()[1], particleIterator2->getParticleDirection()[2]);
                            //adjust speed for gravity
                            particleIterator2->setParticleSpeed(particleIterator2->getParticleSpeed()*gravity);
                        }
                        //now recalculate the new positions
                        newX = particleIterator2->getParticlePosition()[0] + particleIterator2->getParticleDirection()[0]*particleIterator2->getParticleSpeed();
                        newY = particleIterator2->getParticlePosition()[1] + particleIterator2->getParticleDirection()[1]*particleIterator2->getParticleSpeed();
                        newZ = particleIterator2->getParticlePosition()[2] + particleIterator2->getParticleDirection()[2]*particleIterator2->getParticleSpeed();
                    }

                    else if(particleIterator2->getParticleDirection()[1] <= 0){
                        if(newY <= terrain.getHeight(newX, newZ)){

                            terrain.burnTerrain(particleIterator2->getParticlePosition()[0],particleIterator2->getParticlePosition()[2]);

                            particleIterator2->setParticleDirection(particleIterator2->getParticleDirection()[0]*2,
                                                                    particleIterator2->getParticleDirection()[1],
                                                                    particleIterator2->getParticleDirection()[2]*1.05);
                            //friction will take some of the speed away
                            particleIterator2->setParticleSpeed(friction*particleIterator2->getParticleSpeed());

                            if(terrain.getHeight((int) newX, (int) newZ) <=3){
                                terrain.snowTerrain(particleIterator2->getParticlePosition()[0],particleIterator2->getParticlePosition()[2]);
                            }

                            newX = particleIterator2->getParticlePosition()[0] + particleIterator2->getParticleDirection()[0]*particleIterator2->getParticleSpeed();

                            newZ = particleIterator2->getParticlePosition()[2] + particleIterator2->getParticleDirection()[2]*particleIterator2->getParticleSpeed();
                            newY = terrain.getHeight(newX, newZ);//particleIterator2->getParticlePosition()[1] + particleIterator2->getParticleDirection()[1]*particleIterator2->getParticleSpeed();

                        }

                        else{
                            particleIterator2->setParticleSpeed(particleIterator2->getParticleSpeed()*gravity);
                        }
                    }
                }
                //the platform is out of bounds and thus doesn't bounce anymore
                else{

                    newX = particleIterator2->getParticlePosition()[0] + particleIterator2->getParticleDirection()[0]*particleIterator2->getParticleSpeed();
                    newY = particleIterator2->getParticlePosition()[1] + particleIterator2->getParticleDirection()[1]*particleIterator2->getParticleSpeed();
                    newZ = particleIterator2->getParticlePosition()[2] + particleIterator2->getParticleDirection()[2]*particleIterator2->getParticleSpeed();

                    //everytime the direction is negative, let gravity increase the speed at which the particle moves
                    if (particleIterator2->getParticleDirection()[1] < 0){
                            particleIterator2->setParticleSpeed(particleIterator2->getParticleSpeed()*gravity);
                    }
                    if(particleIterator2->getParticleDirection()[1] > 0){
                        particleIterator2->setParticleSpeed(particleIterator2->getParticleSpeed()/gravity);

                        //if the difference is infintismely small and positive (to confirm the particle is still moving up), reverse the direction
                        if ((newY - particleIterator2->getParticlePosition()[1] < 0.05) && (newY- particleIterator2->getParticlePosition()[1] > 0)){
                            particleIterator2->setParticleDirection(particleIterator2->getParticleDirection()[0],
                                                                    (-1)*particleIterator2->getParticleDirection()[1], particleIterator2->getParticleDirection()[2]);
                            //adjust speed for gravity
                            particleIterator2->setParticleSpeed(particleIterator2->getParticleSpeed()*gravity);
                        }
                        //now recalculate the new positions
                        newY = terrain.getHeight(newX, newZ);//newY = particleIterator2->getParticlePosition()[1] + particleIterator2->getParticleDirection()[1]*particleIterator2->getParticleSpeed();
                        newX = particleIterator2->getParticlePosition()[0] + particleIterator2->getParticleDirection()[0]*particleIterator2->getParticleSpeed();

                        newZ = particleIterator2->getParticlePosition()[2] + particleIterator2->getParticleDirection()[2]*particleIterator2->getParticleSpeed();
                    }
                }
                //finally update the particle's position, direction, age
                particleIterator2->setParticlePosition(newX,newY,newZ);
                particleIterator2->setParticleAge(particleIterator2->getParticleAge() + 1);

            }
            else {

                particleIterator2 = particleList.erase(particleIterator2);
                particleIterator2--;
            }
        }
    }
    if(particleType == 0){

        for(particleIterator2 = particleList.begin();particleIterator2 != particleList.end();particleIterator2++){

            if (particleIterator2->getParticleAge() < ageLimit){

                float newX = particleIterator2->getParticlePosition()[0];
                float newY = particleIterator2->getParticlePosition()[1] + particleIterator2->getParticleDirection()[1]*particleIterator2->getParticleSpeed();
                float newZ = particleIterator2->getParticlePosition()[2];

                //particle boundaries
                if ((newX <=particleBounds[1] && newX >particleBounds[0]) && (newZ <=particleBounds[5] && newZ > particleBounds[4])){

                    //if two particles collide, let them form one larger particle
                    if(abs(particleIterator2->getParticlePosition()[0] - newX) < 2 &&
                       abs(particleIterator2->getParticlePosition()[1] - newY) < 2 &&
                       abs(particleIterator2->getParticlePosition()[2] - newZ) < 2){

                        particleIterator2->setParticleSize(particleIterator2->getParticleSize()*1.005);
                    }

                    particleIterator2->setParticleSpeed(particleIterator2->getParticleSpeed());
                    newX = particleIterator2->getParticlePosition()[0];
                    newY = particleIterator2->getParticlePosition()[1] + particleIterator2->getParticleDirection()[1]*particleIterator2->getParticleSpeed();
                    newZ = particleIterator2->getParticlePosition()[2];

                }
                //if the particle touches the platform, let it collect.
                if(newY <= terrain.getHeight((int) newX, (int) newZ)){
                    newY = terrain.getHeight((int) newX, (int) newZ);
                    terrain.snowTerrain(newX, newZ);
                }
                //finally update the particle's position, direction, age
                particleIterator2->setParticlePosition(newX,newY,newZ);
                particleIterator2->setParticleAge(particleIterator2->getParticleAge() + 1);
            }

            else{
                //first raise height to imitate snow buildup
                particleIterator2 = particleList.erase(particleIterator2);
                particleIterator2--;
            }
        }
    }
}
void DrawParticles(void)
{
    if(snowMode){
        CreateParticles(0);
        for(particleIterator3 = particleList.begin();particleIterator3 != particleList.end();particleIterator3++){
            glPushMatrix();
                glTranslatef(particleIterator3->getParticlePosition()[0], particleIterator3->getParticlePosition()[1], particleIterator3->getParticlePosition()[2]);
                //glTranslatef((*particleIterator3).getParticlePosition()[0], (*particleIterator3).getParticlePosition()[1], (*particleIterator3).getParticlePosition()[2])
                glRotatef(particleIterator3->getParticleRotAngle()[0]+angle, 1, 0, 0);
                glRotatef(particleIterator3->getParticleRotAngle()[1]+angle2, 0, 1, 0);
                glRotatef(particleIterator3->getParticleRotAngle()[2]+angle3, 0, 0, 1);
                glScalef(particleIterator3->getParticleSize(), particleIterator3->getParticleSize(), particleIterator3->getParticleSize());

                glutSolidSphere(1,8,4);

            glPopMatrix();
        }
    }
    if(lavaMode){
        CreateParticles(1);
        for(particleIterator3 = lavaParticleList.begin();particleIterator3 != lavaParticleList.end();particleIterator3++){
            glPushMatrix();
                glTranslatef(particleIterator3->getParticlePosition()[0], particleIterator3->getParticlePosition()[1], particleIterator3->getParticlePosition()[2]);
                //glTranslatef((*particleIterator3).getParticlePosition()[0], (*particleIterator3).getParticlePosition()[1], (*particleIterator3).getParticlePosition()[2])
                glRotatef(particleIterator3->getParticleRotAngle()[0]+angle, 1, 0, 0);
                glRotatef(particleIterator3->getParticleRotAngle()[1]+angle2, 0, 1, 0);
                glRotatef(particleIterator3->getParticleRotAngle()[2]+angle3, 0, 0, 1);
                glScalef(particleIterator3->getParticleSize(), particleIterator3->getParticleSize(), particleIterator3->getParticleSize());

                glutSolidSphere(1,20,16);
            glPopMatrix();
        }
    }
    if(steamMode){
        CreateParticles(2);
        for(particleIterator3 = steamParticleList.begin();particleIterator3 != steamParticleList.end();particleIterator3++){
            glPushMatrix();
                glTranslatef(particleIterator3->getParticlePosition()[0], particleIterator3->getParticlePosition()[1], particleIterator3->getParticlePosition()[2]);
                //glTranslatef((*particleIterator3).getParticlePosition()[0], (*particleIterator3).getParticlePosition()[1], (*particleIterator3).getParticlePosition()[2])
                glRotatef(particleIterator3->getParticleRotAngle()[0]+angle, 1, 0, 0);
                glRotatef(particleIterator3->getParticleRotAngle()[1]+angle2, 0, 1, 0);
                glRotatef(particleIterator3->getParticleRotAngle()[2]+angle3, 0, 0, 1);
                glScalef(particleIterator3->getParticleSize(), particleIterator3->getParticleSize(), particleIterator3->getParticleSize());

                glutSolidSphere(1,20,16);
            glPopMatrix();
        }
    }
}

/*****************************************
* draws scene
****************************************/
void display(void) {

    //clear bits and model view matrix
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //transform according to camera
    glRotatef(camera.rotation[0], 1, 0, 0);
    glRotatef(camera.rotation[1], 0, 1, 0);
    glTranslatef(-camera.position[0], -terrain.getHeight(camera.position[0], camera.position[2])-3, -camera.position[2]);
    DrawParticles();
    fireParticles.DrawParticles();

    //draw the scene
    drawAxes();
    terrain.drawTerrain();
    //volcanoParticles.drawParticles();

    glutSwapBuffers();
}

void drawAxes() {
    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(0,0,0);
    glVertex3f(500, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 500, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 500);
    glEnd();

    glEnable(GL_LIGHTING);
}

/********************************************
* handles key presses for program functions
*******************************************/
void keyboard(unsigned char key, int x, int y) {

    switch (key) {

        //quit
        case 'q':
            exit(0);
            break;

        //pause
        case 'p':
        case 'P':
            paused = !paused;
            break;

        //toggle fullscreen
        case 'f':
        case 'F':
            if (!fullscreen)
                glutFullScreen();
            else {
                glutPositionWindow(10, 10);
                glutReshapeWindow(800, 600);
            }
            break;

        case 'r':
            terrain.generateTerrain();
//            volcanoParticles = ParticleSystem(terrain);
            break;
        case '1':
            glShadeModel(GL_FLAT);
            break;
        case '2':
            glShadeModel(GL_SMOOTH);
            break;

        //move player
        case 'w':
        case 'W':
            camera.strafe(Camera::FORWARD);
            break;
        case 's':
        case 'S':
            camera.strafe(Camera::BACK);
            break;
        case 'a':
        case 'A':
            camera.strafe(Camera::LEFT);
            break;
        case 'd':
        case 'D':
            camera.strafe(Camera::RIGHT);
            break;
    }
    glutPostRedisplay();
}

/********************************************
* moves camera (First Person)
*******************************************/
void passive(int x, int y) {

    //on first call set currX, currY
    if (!mouseCurrentInitiated) {
        currX = x;
        currY = y;
        mouseCurrentInitiated = true;
    }

    //if cursor approaching edge, set it to middle of window
    if (x < 1 || x >= windowWidth-1) {
        glutWarpPointer(windowWidth/2.0, y);
        currX = windowWidth/2.0;
    }
    if (y < 1 || y >= windowHeight-1) {
        glutWarpPointer(x, windowHeight/2.0);
        currY = windowHeight/2.0;
    }

    //move camera according to mouse movement
    camera.mouseMoved(x-currX, y-currY);
    currX = x;
    currY = y;

    glutPostRedisplay();
}

/********************************************
* moves volcano particles
*******************************************/
void timer(int value) {

    if (!paused)
        //volcanoParticles.moveParticles();

    //set timer function
    glutTimerFunc(32, timer, 0);
    if(snowMode) UpdateParticles(0);
    if(lavaMode) UpdateParticles(1);
    if(steamMode) UpdateParticles(2);
    fireParticles.UpdateParticles();
    glutPostRedisplay();
}


/********************************************
* sets viewport according to window size
*******************************************/
void reshape(int w, int h) {

    //don't let window become less than 300 x 300
    int minWindowSize = 300;
    if (w < minWindowSize || h < minWindowSize)
        glutReshapeWindow((w < minWindowSize) ? minWindowSize : w, (h < minWindowSize) ? minWindowSize : h);

    //change projection matrix, set width & height globals
    else {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glViewport(0, 0, (GLsizei) w, (GLsizei) h);
        gluPerspective(45, (GLfloat) w / (GLfloat) h, 1,400);

        windowWidth = w;
        windowHeight = h;
    }

    glutPostRedisplay();
}

/*******************************************
*initializes global variables and settings
******************************************/
void init() {

    //enable back face culling & flat shading (for artistic reasons)
    glClearColor(0.1, 0.1, 0.1, 1);
    glEnable(GL_DEPTH_TEST);
   // glEnable(GL_CULL_FACE);
    glShadeModel(GL_FLAT);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);



    //set projection matrix, using perspective w/ correct aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,(GLfloat) glutGet(GLUT_WINDOW_WIDTH) / (GLfloat) glutGet(GLUT_WINDOW_HEIGHT), 1, 400);

    //initialize globals
    terrain = Terrain();
    //volcanoParticles = ParticleSystem(&terrain);
    //volcanoParticles.emitterPos[0] = 0;
    //volcanoParticles.emitterPos[1] = terrain.getHeight(0,0)+1;
    //volcanoParticles.emitterPos[2] = 0;

    //initialize camera
    camera = Camera();

    //hide cursor
    glutSetCursor(GLUT_CURSOR_NONE);
}

/*****************************************
* program start point
****************************************/
int main(int argc, char** argv) {

    //initializeing GLUT
    glutInit(&argc, argv);

    //making our window
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Volcano");

    //initializing variables
    init();

    //registering callbacks
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(passive);
    glutTimerFunc(32, timer, 0);

    //start event loop
    glutMainLoop();

    return(0);
}
