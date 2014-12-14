// CS 3GC3 Final Project
//
// Camera.cpp
// -handles FPS camera movements
// -arrow keys move wrt where camera is
// pointing.
// -moving mouse will move camera
//
// This is a heavily modified version of the camera class found on:
// r3dux.org/2012/12/a-c-camera-class-for-simple-opengl-fps-controls
//
// We have rewritten almost all of it to suit our needs, however we
// wanted to cite the original source.


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

#include <math.h>
#include <stdio.h>

#include "Camera.h"

/****************************************
 * Constructor
 ****************************************/
Camera::Camera() {
    
    //move camera slightly off center
    position[0] = 0.5;
    position[1] = 0;
    position[2] = 20;
}

/****************************************
 * rotates camera view depending on where
 * where mouse was moved
 ****************************************/
void Camera::mouseMoved(int deltaX, int deltaY) {
    
    //rotate camera
    rotation[0] += ((float) deltaY) * mouseSensitivity;
    rotation[1] += ((float) deltaX) * mouseSensitivity;
    
    // Limit looking down to vertically down
    if (rotation[0] > 90) {
        rotation[0] = 90;
    }
    if (rotation[0] < -90) {
        rotation[0] = -90;
    }
    
    if (rotation[1] < 0) {
        rotation[1] += 360;
    }
    if (rotation[1] > 360) {
        rotation[1] += -360;
    }
}

/****************************************
 * moves the camera in the direction that
 * is passed as an argument, wrt where
 * camera is pointing
 ****************************************/
void Camera::strafe(Direction dir, bool sprinting) {
    
    //vector to add to our position
    float movementVec[3] = {0,0,0};
    
    //calculate rotations (need to move in direction we're pointing)
    float cosXRot = cos(rotation[0]*TO_RADS);
    float sinYRot = sin(rotation[1]*TO_RADS);
    float cosYRot = cos(rotation[1]*TO_RADS);
    
    //move camera
    if (dir == FORWARD) {
        movementVec[0] += sinYRot*cosXRot;
        movementVec[2] += -cosYRot*cosXRot;
    }
    else if (dir == BACK) {
        movementVec[0] += -sinYRot*cosXRot;
        movementVec[2] += cosYRot*cosXRot;
    }
    else if (dir == LEFT) {
        movementVec[0] += -cosYRot;
        movementVec[2] += -sinYRot;
    }
    else if (dir == RIGHT) {
        movementVec[0] += cosYRot;
        movementVec[2] += sinYRot;
    }
    
    //calculate length of vector
    float movementVecLength = sqrtf(movementVec[0]*movementVec[0] + movementVec[1]*movementVec[1] + movementVec[2]*movementVec[2]);
    
    //move faster if sprinting
    if (sprinting) {
        movementVec[0] *= 3;
        movementVec[2] *= 3;
    }
    
    //normalize vector
    position[0] += (movementVec[0]/movementVecLength)*movementSpeedFactor;
    position[2] += (movementVec[2]/movementVecLength)*movementSpeedFactor;
}
