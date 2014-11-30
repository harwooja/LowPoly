#ifndef _OPENGL_
#define _OPENGL_
#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <windows.h>
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif
#endif

#ifndef _STANDARD_
#define _STANDARD_
#include <vector>
#include <stdlib.h>
#include <math.h>
#endif

#include "Terrain.h"
/*****************************************
 *    FUNCTION DECLARATIONS
 ****************************************/
void drawAxes();

/*****************************************
 *    GLOBAL VARIABLES
 ****************************************/
Terrain terrain;
bool paused = false;
float camPos[3] = {-50,50,-50};
float camLookAt[3] = {75,0,75};
float lightPos[4] = {30,30,30, 1};

/*****************************************
 * displays all objects
 ****************************************/
void display(void) {
    
    //clear bits and model view matrix
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //point camera
    gluLookAt(camPos[0],camPos[1],camPos[2], camLookAt[0],camLookAt[1],camLookAt[2], 0,1,0);

    drawAxes();
    terrain.drawTerrain();
    
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
       case 'q':
            exit(0);
            break;
        case '[':
            camPos[2] -= 1;
            break;
        case ']':
            camPos[2] += 1;
            break;
    }
    
    glutPostRedisplay();
}

/*****************************************
 * handles arrow key presses (to move cam)
 ****************************************/
void special(int key, int x, int y) {
    
    //move camera w/ arrow keys
    switch(key) {
        case GLUT_KEY_LEFT:
            camPos[0] -= 1;
            break;
            
        case GLUT_KEY_RIGHT:
            camPos[0] += 1;
            break;
            
        case GLUT_KEY_UP:
            camPos[1] += 1;
            break;
            
        case GLUT_KEY_DOWN:
            camPos[1] -= 1;
            break;
    }
    glutPostRedisplay();
}

/********************************************
 * sets viewport according to window size
 *******************************************/
void reshapeFunc(int w, int h) {
    
    //don't let window become less than 300 x 300
    int minWindowSize = 300;
    if (w < minWindowSize || h < minWindowSize)
        glutReshapeWindow((w < minWindowSize) ? minWindowSize : w, (h < minWindowSize) ? minWindowSize : h);
    
    else {
        
        //set projection matrix, using perspective
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        
        //set up viewport
        glViewport(0, 0, (GLsizei) w, (GLsizei) h);
        gluPerspective(45, (GLfloat) w / (GLfloat) h, 1, 100);
    }
    
    glutPostRedisplay();
}

/*******************************************
 *initializes global variables and settings
 ******************************************/
void init() {
    
    //enable back face culling
    //glEnable(GL_CULL_FACE);
    
    glClearColor(0.1, 0.1, 0.1, 0);

    glShadeModel(GL_FLAT);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    
    //set projection matrix, using perspective w/ correct aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,(GLfloat) glutGet(GLUT_WINDOW_WIDTH) / (GLfloat) glutGet(GLUT_WINDOW_HEIGHT), 1, 100);
    
    terrain = Terrain();
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
    
    //registering callbacks
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshapeFunc);
    glutSpecialFunc(special);
    
    //setting up depth test & lighting normalization
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
    //initializing variables
    init();
    
    //start event loop
    glutMainLoop();
    
    return(0);
}