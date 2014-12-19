// CS 3GC3 Final Project
//
// main.cpp
// -program entry-point
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
#include <vector>
#include <string>
#include "ParticleList.h"
#include "ResourceLoader.h"
#include "Camera.h"


/*****************************************
 *    FUNCTION DECLARATIONS
 ****************************************/
void drawPauseMenu();
void togglePausedScene();
void passive(int x, int y);
void timer(int value);
void drawSkybox();

/*****************************************
 *    GLOBAL VARIABLES
 ****************************************/
Terrain terrain;
Camera camera;

ParticleList snowParticles = ParticleList(ParticleList::SNOW, &terrain);
ParticleList lavaParticles = ParticleList(ParticleList::LAVA, &terrain);

//state
bool paused = false;
bool fullscreen = false;
float lightPos[4] = {0,65,0, 1};

//used for passive func
bool mouseCurrentInitiated = false;
int currX = 0;
int currY = 0;
int windowWidth = 800;
int windowHeight = 600;

//pause menu
int pauseMenuWidth = 0;
int pauseMenuHeight = 0;
GLubyte *pauseMenuImage;

GLubyte *frontTex;
GLubyte *topTex;
GLubyte *leftTex, *rightTex, *backTex;
GLuint textures[5];

GLuint fogMode[] = { GL_EXP, GL_EXP2, GL_LINEAR }; //storage for three types of fog
int fogfilter = -1; //which fog to use
float fogColor[4] = {0.5,0.5,0.5, 1.0}; //fog colour

bool perspectiveMode = true;

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

    //draw the scene
    terrain.drawTerrain();
    lavaParticles.drawAndAddParticles();
    snowParticles.drawAndAddParticles();
    drawSkybox();

    if (paused)
        drawPauseMenu();

    glutSwapBuffers();
}
void orthoDisplay()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-200, 200, -200, 200, -200, 200);
}
void perspectiveDisplay()
{
    //change to projection matrix mode, set the extents of our viewing volume
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
    gluPerspective(45,(GLfloat) glutGet(GLUT_WINDOW_WIDTH) / (GLfloat) glutGet(GLUT_WINDOW_HEIGHT), 1, 300);

}
/********************************************
 * draws the menu for when game paused
 *******************************************/
void drawPauseMenu() {

    glDisable(GL_DEPTH_TEST);

    //set projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);

    //draw pixels of image
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRasterPos2i(windowWidth/2+pauseMenuWidth/2, windowHeight/2-pauseMenuHeight/2);
    glPixelZoom(-1,1);
    if (pauseMenuImage != NULL)
        glDrawPixels(pauseMenuWidth, pauseMenuHeight, GL_RGB, GL_UNSIGNED_BYTE, pauseMenuImage);

    //reset projection matrixs
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float) windowWidth / (float) windowHeight, 1,360);

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
    if (paused) {
        glutPassiveMotionFunc(NULL);
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        lavaParticles.paused = true;
        snowParticles.paused = true;
    }

    //show cursor, disable moving camera
    else {
        glutPassiveMotionFunc(passive);
        glutSetCursor(GLUT_CURSOR_NONE);
        lavaParticles.paused = false;
        snowParticles.paused = false;
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

        //fog
        case 'g':
        case 'G':
            fogfilter += 1;
            if (fogfilter > -1 && fogfilter < 2) {
                glEnable(GL_FOG);
                glFogi(GL_FOG_MODE, fogMode[fogfilter]);
            }
            else if (fogfilter > 2)
                fogfilter = -1;

            if (fogfilter==-1)
                glDisable(GL_FOG);
            break;
<<<<<<< HEAD
        case 'm':
        case 'M':
            if(perspectiveMode == true) orthoDisplay();
            else if (perspectiveMode == false) perspectiveDisplay();
            perspectiveMode = !perspectiveMode;
=======
>>>>>>> e0d29611993d7926a55f888413d8a6a4dc13015b
    }

    //keys that only work when not paused
    if (!paused) {
        switch (key) {

            //move player
            case 'w':
            case 'W':
                if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
                    camera.strafe(Camera::FORWARD, true);
                else
                    camera.strafe(Camera::FORWARD, false);
                break;
            case 's':
            case 'S':
                if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
                    camera.strafe(Camera::BACK, true);
                else
                    camera.strafe(Camera::BACK, false);
                break;
            case 'a':
            case 'A':
                if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
                    camera.strafe(Camera::LEFT, true);
                else
                    camera.strafe(Camera::LEFT, false);
                break;
            case 'd':
            case 'D':
                if (glutGetModifiers() == GLUT_ACTIVE_SHIFT)
                    camera.strafe(Camera::RIGHT, true);
                else
                    camera.strafe(Camera::RIGHT, false);
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
        int leftHud = windowWidth/2 - pauseMenuWidth/2;
        int rightHud = windowWidth/2 + pauseMenuWidth/2;
        int bottomHud = windowHeight/2 + pauseMenuHeight/2;
        int topHud = windowHeight/2 - pauseMenuHeight/2;

        //click is within hud, with 20px padding
        if (x > leftHud+20 && x < rightHud-20 && y > topHud+20 && y < bottomHud-20) {

            //top button - toggle lava
            if (y > topHud+30 && y < topHud+140) {

                lavaParticles.enabled = !lavaParticles.enabled;

                //clear particles or add some new ones
                if (!lavaParticles.enabled)
                    lavaParticles.clearParticles();
                if (lavaParticles.enabled)
                    for (int i = 0; i < 10; i++)
                        lavaParticles.addParticle();
            }

            //second button - toggle snow
            else if (y > topHud+170 && y < topHud+280) {

                snowParticles.enabled = !snowParticles.enabled;

                //clear particles or add some new ones
                if (!snowParticles.enabled)
                    snowParticles.clearParticles();
                if (snowParticles.enabled)
                    for (int i = 0; i < 10; i++)
                        snowParticles.addParticle();
            }

            //bottom buton
            else if (y < bottomHud-50 && y > bottomHud-105)
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
        lavaParticles.updateParticles();
        snowParticles.updateParticles();
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
    int minWindowSize = 550;
    if (w < minWindowSize || h < minWindowSize) {
        glutReshapeWindow((w < minWindowSize) ? minWindowSize : w, (h < minWindowSize) ? minWindowSize : h);
        windowWidth = (w < minWindowSize) ? 550 : w;
        windowHeight = (h < minWindowSize) ? 550 : h;
    }

    //change projection matrix, set width & height globals
    else {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        glViewport(0, 0, (GLsizei) w, (GLsizei) h);
        gluPerspective(45, (GLfloat) w / (GLfloat) h, 1, 360);

        windowWidth = w;
        windowHeight = h;
    }

    glutPostRedisplay();
}

/********************************************
* draws textured skybox around scene
*******************************************/
void drawSkybox() {

    glDisable(GL_LIGHTING);

    int height = 128;
    int width = 132;

    //FRONT (0) and BACK (1)
    for (int i = 0; i < 2; i++) {
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glBegin(GL_QUADS);

        glTexCoord2f(0, 0);
        glVertex3f(width*pow(-1,i), 0, width*pow(-1,i));

        glTexCoord2f(0, 1);
        glVertex3f(width*pow(-1,i), height, width*pow(-1,i));

        glTexCoord2f(1, 1);
        glVertex3f(width*pow(-1,i), height, -width*pow(-1,i));

        glTexCoord2f(1, 0);
        glVertex3f(width*pow(-1,i), 0, -width*pow(-1,i));

        glEnd();
    }

    //LEFT (2) and RIGHT (3)
    for (int i = 2; i < 4; i++) {
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glBegin(GL_QUADS);

        glTexCoord2f(0, 0);
        glVertex3f(-width*pow(-1,i), 0, width*pow(-1,i));

        glTexCoord2f(0, 1);
        glVertex3f(-width*pow(-1,i), height, width*pow(-1,i));

        glTexCoord2f(1, 1);
        glVertex3f(width*pow(-1,i), height, width*pow(-1,i));

        glTexCoord2f(1, 0);
        glVertex3f(width*pow(-1,i), 0, width*pow(-1,i));
        glEnd();

    }

    //TOP (4)
    glBindTexture(GL_TEXTURE_2D, textures[4]);
    glBegin(GL_QUADS);

    glTexCoord2f(0, 0);
    glVertex3f(width, height, width);

    glTexCoord2f(0, 1);
    glVertex3f(-width, height, width);

    glTexCoord2f(1, 1);
    glVertex3f(-width, height, -width);

    glTexCoord2f(1, 0);
    glVertex3f(width, height, -width);
    glEnd();

    glEnable(GL_LIGHTING);
    glBindTexture(GL_TEXTURE_2D, NULL);
}

/*******************************************
* initializes global variables and settings
******************************************/
void init() {

    //enable textures (for skybox)
    glEnable(GL_TEXTURE_2D);
    glGenTextures(5, textures);

    //load textures
    ResourceLoader imageLoader = ResourceLoader();
    imageLoader.loadPPMTexture((char*)"/images/skybox_front.ppm", true, &textures[0]);
    imageLoader.loadPPMTexture((char*)"/images/skybox_back.ppm", true, &textures[1]);
    imageLoader.loadPPMTexture((char*)"/images/skybox_left.ppm", true, &textures[2]);
    imageLoader.loadPPMTexture((char*)"/images/skybox_right.ppm", true, &textures[3]);
    imageLoader.loadPPMTexture((char*)"/images/skybox_top.ppm", true, &textures[4]);

    //enable flat shading (for artistic reasons)
    glShadeModel(GL_FLAT);

    glClearColor(0.21, 0.53, 0.77, 1);
    glEnable(GL_DEPTH_TEST);

    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    //fog not enabled by default
    glFogfv(GL_FOG_COLOR, fogColor);
    glFogf(GL_FOG_DENSITY, 0.05);
    glFogf(GL_FOG_START, 3.0);
    glFogf(GL_FOG_END, 10.0);


    //messing around with blending just ignore this
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);


    //set projection matrix, using perspective w/ correct aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,(GLfloat) glutGet(GLUT_WINDOW_WIDTH) / (GLfloat) glutGet(GLUT_WINDOW_HEIGHT), 1, 300);

    //initialize globals
    terrain = Terrain();

    //initialize camera
    camera = Camera();

    //setup interface image
    pauseMenuImage = imageLoader.loadPPM((char*) "/images/pause_menu.ppm", true, &pauseMenuWidth, &pauseMenuHeight);

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
