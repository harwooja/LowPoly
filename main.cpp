// CS 3GC3 Final Project
//
// main.cpp
// -program entrypoiny
// -ViewController in MVC terms
// -draws scene, handles interaction, etc.

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
#include <math.h>

#include "Particle.h"
#include "ParticleList.h"
#include "ParticleSystem.h"
#include "ImageLoader.h"
#include "Camera.h"


/*****************************************
*    FUNCTION DECLARATIONS
****************************************/
void drawAxes();
void drawHud();
void togglePausedScene();
void passive(int x, int y);
void timer(int value);

/*****************************************
 *    GLOBAL VARIABLES
 ****************************************/
Terrain terrain;
ParticleSystem volcanoParticles(&terrain);
Camera camera;

bool fullscreen = false;
bool paused = false;
float lightPos[4] = {0,60,0, 1};

bool mouseCurrentInitiated = false;
int currX = 0;
int currY = 0;
int windowWidth = 800;
int windowHeight = 600;

//xmin,xmax,ymin,ymax,zmin,zmax
float particleBounds[] = {-50,50,4,50,-50,50};

// 0 is snow, 1 is lava, 2 is steam(still in development)
ParticleList snowParticles(0,particleBounds,&terrain);
ParticleList fireParticles(1,particleBounds,&terrain);

int hudWidth = 0;
int hudHeight = 0;
GLubyte *hudImage;

bool birdsEyeView = false;
bool testingVectorParticles = false;

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
    
    if (!birdsEyeView)
        glTranslatef(-camera.position[0], -terrain.getHeight(camera.position[0], camera.position[2])-3, -camera.position[2]);
    else
        glTranslatef(-camera.position[0], -100, -camera.position[2]);

    //draw the scene
    terrain.drawTerrain();
    if (testingVectorParticles)
        volcanoParticles.drawParticles();
    else {
        fireParticles.DrawParticles();
        //snowParticles.DrawParticles();
    }

    glTranslatef(25, 40, 58);
    
    if (paused)
        drawHud();

    glutSwapBuffers();
}

/********************************************
* draws the menu for when game paused
*******************************************/
void drawHud() {
    
    glDisable(GL_DEPTH_TEST);
    
    //set projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    
    //draw pixels of image
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRasterPos2i(windowWidth/2+hudWidth/2, windowHeight/2-hudHeight/2);
    glPixelZoom(-1,1);
    if (hudImage != NULL)
        glDrawPixels(hudWidth, hudHeight, GL_RGB, GL_UNSIGNED_BYTE, hudImage);
    
    //reset projection matrixs
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float) windowWidth / (float) windowHeight, 1,300);
    
    glEnable(GL_DEPTH_TEST);
}

/********************************************
* pauses or unpauses scene. If pausing, draws
* the menu (hud) and disables interactivity
* except with the menu or unpausing or quitting
*******************************************/
void togglePausedScene() {

    paused = !paused;

    //reenable moving camera and hide cursor
    if (!paused) {
        glutPassiveMotionFunc(passive);
        glutSetCursor(GLUT_CURSOR_NONE);
    }
    
    //show cursor, disable movign camera
    else {
        glutPassiveMotionFunc(NULL);
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }
}

/********************************************
* handles key presses for program functions
*******************************************/
void keyboard(unsigned char key, int x, int y) {

    //keys that are handled whether paused or not
    switch (key) {

        //quit
        case 'q':
            exit(0);
            break;

        //pause
        case 'p':
        case 'P':
        case 27:
            togglePausedScene();
            break;

        //toggle fullscreen
        case 'f':
        case 'F':
            fullscreen = !fullscreen;
            if (!fullscreen)
                glutFullScreen();
            else {
                glutPositionWindow(10, 10);
                glutReshapeWindow(800, 600);
            }
            break;
    }

    //keys that only work when not paused
    if (!paused) {
        switch (key) {
            
            //change global state
            case '0':
                testingVectorParticles = !testingVectorParticles;
                break;
            case 'b':
                birdsEyeView = !birdsEyeView;
                break;
            case '1':
                glShadeModel(GL_FLAT);
                break;
            case '2':
                glShadeModel(GL_SMOOTH);
                break;
            case '3':
                volcanoParticles.shape = ParticleSystem::CUBE;
                break;
            case '4':
                volcanoParticles.shape = ParticleSystem::SPHERE;
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
 * handles menu clicks when paused
 *******************************************/
void mouse(int button, int state, int x, int y) {
    
    if (state == GLUT_LEFT_BUTTON) {
        
        //get bounds of hud
        int leftHud = windowWidth/2 - hudWidth/2;
        int rightHud = windowWidth/2 + hudWidth/2;
        int bottomHud = windowHeight/2+hudHeight/2;
        int topHud = windowHeight/2-hudHeight/2;
        
        //click is within hud, with 20px padding
        if (x > leftHud+20 && x < rightHud-20 && y > topHud+20 && y < bottomHud-20) {
            
            //top button
            if (y > topHud+30 && y < topHud+140) {
                printf("top button");
                //flatShading = !flatShading;
                //if (!flatShading)
                //    glShadeModel(GL_SMOOTH);
                //else
                //    glShadeModel(GL_FLAT);
            }
            
            //second button
            else if (y > topHud+170 && y < topHud+280) {
                printf("2nd button");
                //if (volcanoParticles.shape == ParticleSystem::CUBE)
                //    volcanoParticles.shape = ParticleSystem::SPHERE;
                //else
                //    volcanoParticles.shape = ParticleSystem::CUBE;
            }
            
            //bottom buton
            else if (y <bottomHud-50 && y > bottomHud-105)
                exit(1);
        }
    }
}

/********************************************
* moves volcano particles
*******************************************/
void timer(int value) {

    //update particles
    if (!paused) {
        if (testingVectorParticles)
            volcanoParticles.moveParticles();
        else {
            fireParticles.UpdateParticles();
            //snowParticles.UpdateParticles(terrain);
        }
    }
    
    //set timer function
    glutTimerFunc(32, timer, 0);
    glutPostRedisplay();
}

/********************************************
* sets viewport according to window size
*******************************************/
void reshape(int w, int h) {

    //don't let window become less than 300 x 300
    int minWindowSize = 300;
    if (w < minWindowSize || h < minWindowSize) {
        glutReshapeWindow((w < minWindowSize) ? minWindowSize : w, (h < minWindowSize) ? minWindowSize : h);
        windowWidth = (w < minWindowSize) ? 300 : w;
        windowHeight = (h < minWindowSize) ? 300 : h;
    }
    
    //change projection matrix, set width & height globals
    else {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glViewport(0, 0, (GLsizei) w, (GLsizei) h);
        gluPerspective(45, (GLfloat) w / (GLfloat) h, 1, 300);

        windowWidth = w;
        windowHeight = h;
    }

    glutPostRedisplay();
}

/*******************************************
*initializes global variables and settings
******************************************/
void init() {

    //enable flat shading (for artistic reasons)
    glShadeModel(GL_FLAT);
    
    glClearColor(0.25, 0.53, 0.77, 1);
    glEnable(GL_DEPTH_TEST);
    
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    
    //set projection matrix, using perspective w/ correct aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,(GLfloat) glutGet(GLUT_WINDOW_WIDTH) / (GLfloat) glutGet(GLUT_WINDOW_HEIGHT), 1, 300);
    
    //initialize globals
    terrain = Terrain();
    snowParticles = ParticleList(0,particleBounds,&terrain);
    fireParticles = ParticleList(1,particleBounds,&terrain);
    volcanoParticles = ParticleSystem(&terrain);
    
    //setup interface image
    ImageLoader imgLoader = ImageLoader();
    hudImage = imgLoader.loadPPM((char*) "/interface.ppm", true, &hudWidth, &hudHeight);
    
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
    glutMouseFunc(mouse);
    glutTimerFunc(32, timer, 0);

    //start event loop
    glutMainLoop();

    return(0);
}
