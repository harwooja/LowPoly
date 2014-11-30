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

#include <vector>
#include <stdlib.h>
#include <math.h>
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
float camPos[3] = {-100,60,-100};
float camLookAt[3] = {100,5,100};
float lightPos[4] = {0,50,0, 1};
float currX = 0;
float currY = 0;
float rot = 0;
float rotY = 0;
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

    glRotatef(rot, 0, 1, 0);
    glRotatef(rotY, 1, 0, 0);
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
            
        case 'w':
        case 'W':
            camPos[0] += 1;
            camPos[1] = terrain.getHeight(camPos[0], camPos[2]);
            break;
        case 's':
        case 'S':
            camPos[0] -= 1;
            camPos[1] = terrain.getHeight(camPos[0], camPos[2]);
            break;
        case 'a':
        case 'A':
            camPos[2] -= 1;
            camPos[1] = terrain.getHeight(camPos[0], camPos[2]);
            break;
        case 'd':
        case 'D':
            camPos[2] += 1;
            camPos[1] = terrain.getHeight(camPos[0], camPos[2]);
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

void passive(int x, int y) {
    
    printf("currX: %f x: %d currY: %f, y: %d\n",currX, x, currY, y);
    
    if (x-currX > 0) {
//        glMatrixMode(GL_MODELVIEW);
  //      glRotatef(3, 1, 0, 0);
        rot += 1;
    }
    if (x - currX < 0) {
        rot -= 1;
    }
    if (y-currY > 0) {
        rotY += 1;
    }
    if (y-currY < 0) {
        rotY -= 1;
    }
    currX = x;
    currY = y;
    
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
    
    else {
        
        //set projection matrix, using perspective
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        
        //set up viewport
        glViewport(0, 0, (GLsizei) w, (GLsizei) h);
        gluPerspective(45, (GLfloat) w / (GLfloat) h, 1, 500);
    }
    
    glutPostRedisplay();
}

/*******************************************
 *initializes global variables and settings
 ******************************************/
void init() {
    
    //enable back face culling
    //glEnable(GL_CULL_FACE);
    
    glClearColor(0.1, 0.1, 0.1, 1);

    glShadeModel(GL_FLAT);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    
    //set projection matrix, using perspective w/ correct aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,(GLfloat) glutGet(GLUT_WINDOW_WIDTH) / (GLfloat) glutGet(GLUT_WINDOW_HEIGHT), 1, 100);
    
    terrain = Terrain();
    
    camPos[0] = 0;
    camPos[2] = 0;
    camPos[1] = terrain.getHeight(camPos[0], camPos[2])+5;
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
    glutSpecialFunc(special);
    glutPassiveMotionFunc(passive);
    
    //setting up depth test & lighting normalization
    glEnable(GL_DEPTH_TEST);
    
    //start event loop
    glutMainLoop();
    
    return(0);
}