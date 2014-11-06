#ifndef _OPENGL_
#define _OPENGL_
#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
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

/*****************************************
 *    FUNCTION DECLARATIONS
 ****************************************/


/*****************************************
 *    GLOBAL VARIABLES
 ****************************************/
bool paused = false;


/*****************************************
 * displays all objects
 ****************************************/
void display(void) {
    
    //clear bits and model view matrix
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //point camera
    gluLookAt(-10,10,-10, 0,0,0, 0,1,0);
    
    float diffuse[4] = {1,0.8,0, 1.0};
    float ambient[4] = {1,0.8,0, 1.0};
    float specular[4] = {0.9,0.9,0.9, 1.0};
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 40);
    
    glPopMatrix();
    glutSwapBuffers();
}

/********************************************
 * handles key presses for program functions
 *******************************************/
void keyboard(unsigned char key, int x, int y) {
    
    switch (key) {
            
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
    glEnable(GL_CULL_FACE);
    
    glClearColor(0, 0, 0, 0);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    float lightPos[4] = {-5,4.5,0.5,1};
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    
    //set projection matrix, using perspective w/ correct aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,(GLfloat) glutGet(GLUT_WINDOW_WIDTH) / (GLfloat) glutGet(GLUT_WINDOW_HEIGHT), 1, 100);
}

/*****************************************
 * program start point
 ****************************************/
int main(int argc, char** argv) {
    
    //initializeing GLUT
    glutInit(&argc, argv);
    
    //making our window
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Volcano");
    
    //registering callbacks
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshapeFunc);
    
    //setting up depth test & lighting normalization
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
    //initializing variables
    init();
    
    //start event loop
    glutMainLoop();
    
    return(0);
}