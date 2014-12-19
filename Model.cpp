// CS 3GC3 Final Project
//
// Model.cpp
// -constructor takes path to .obj file
// and loads it
// -stores a 3D model, as well as function
// to draw model

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#ifdef _WIN32
#  include <windows.h>
#endif
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <vector>
#include <stdlib.h>

#include "Model.h"
#include "ResourceLoader.h"

/***************************************
*    GLOBAL VARIABLES
**************************************/



/*****************************************
* Loads the .obj file using ResourceLoader
* and sets state accordingly
****************************************/
Model::Model(char* fileName, bool pathRelative, float scaleFactor, float x, float y, float z, bool randomizeRot, float* modelColour) {
    
    //clear all existing data
    vertices.clear();
    normals.clear();
    faceIndices.clear();
    normalIndices.clear();
    
    //set globals
    scale = scaleFactor;
    xPos = x;
    yPos = y;
    zPos = z;
    colour[0] = modelColour[0];
    colour[1] = modelColour[1];
    colour[2] = modelColour[2];
    colour[3] = modelColour[3];
    
    //generate a random rotation
    if (randomizeRot) {
        rotation = ((double) rand()/RAND_MAX)*360;
        xRot = ((double) rand()/RAND_MAX);
        yRot = ((double) rand()/RAND_MAX);
        zRot = ((double) rand()/RAND_MAX);
    }
    
    //load object file
    ResourceLoader objLoader = ResourceLoader();
    objLoader.loadObj(fileName, pathRelative, &vertices, &normals, &faceIndices, &normalIndices);
}

/*****************************************
* draws our model with correct position
* and scaling
****************************************/
void Model::drawModel() {

    //even though normals are unit vectors, the scaling affects
    //them so we need to normalize
    glEnable(GL_NORMALIZE);
    
    //set colour
    glMaterialfv(GL_FRONT, GL_AMBIENT, colour);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, colour);
    
    glPushMatrix();

    //transform
    glTranslatef(xPos, yPos, zPos);
    glScalef(scale, scale, scale);
    glRotatef(rotation, xRot, yRot, zRot);
    
    //draw all vertices (with vertex normals)
    for (int i = 0; i < faceIndices.size(); i++) {

        glBegin(GL_TRIANGLES);
        
        glNormal3f(normals[normalIndices[i][0]][0], normals[normalIndices[i][0]][1], normals[normalIndices[i][0]][2]);
        glVertex3f(vertices[faceIndices[i][0]][0], vertices[faceIndices[i][0]][1], vertices[faceIndices[i][0]][2]);
        
        glNormal3f(normals[normalIndices[i][1]][0], normals[normalIndices[i][1]][1], normals[normalIndices[i][1]][2]);
        glVertex3f(vertices[faceIndices[i][1]][0], vertices[faceIndices[i][1]][1], vertices[faceIndices[i][1]][2]);
        
        glNormal3f(normals[normalIndices[i][2]][0], normals[normalIndices[i][2]][1], normals[normalIndices[i][2]][2]);
        glVertex3f(vertices[faceIndices[i][2]][0], vertices[faceIndices[i][2]][1], vertices[faceIndices[i][2]][2]);
        
        glEnd();
    }
    
    glPopMatrix();
    
    glDisable(GL_NORMALIZE);
}
