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

#include <vector>
#include <stdlib.h>
#include <math.h>
#include "Terrain.h"
#include "ParticleSystem.h"
#include "Camera.h"


/*****************************************
*    FUNCTION DECLARATIONS
****************************************/
void drawAxes();

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
    drawAxes();
    terrain.drawTerrain();
    volcanoParticles.drawParticles();
    
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
        volcanoParticles.moveParticles();
    
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
    volcanoParticles = ParticleSystem(&terrain);
    volcanoParticles.emitterPos[0] = 0;
    volcanoParticles.emitterPos[1] = terrain.getHeight(0,0)+1;
    volcanoParticles.emitterPos[2] = 0;

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