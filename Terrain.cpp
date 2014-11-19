//
//  Terrain.cpp
//  LowPoly
//
//  Created by Jake Harwood on 2014-11-16.
//  Copyright (c) 2014 Jake Harwood. All rights reserved.
//

//** Stores all vertices of terrain & volcano, vertex orderings, normals, and colours **//

#include "Terrain.h"

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

#define TERRAIN_SIZE 150

float triangles[2][3][3];
float heightMap[TERRAIN_SIZE][TERRAIN_SIZE];

Terrain::Terrain(){
    generateTerrain();
}

void Terrain::generateTerrain() {
    for (int x = 0; x < TERRAIN_SIZE; x++) {
        for (int z = 0; z < TERRAIN_SIZE; z++) {
            heightMap[x][z] = 10;
        }
    }
    
    //more iterations = more jagged (scales linearly with TERRAIN_SIZE)
    int iterations = TERRAIN_SIZE+250;
    
    for (int i = 0; i < iterations; i++) {
        
        //the following code (until the end of the method) taken from
        //www.lighthouse3d.com/opengl/terrain/index.php?impdetails
        //(the website linked in the assignment)
        float v = rand();
        float a = sinf(v);
        float b = cosf(v);
        float d = sqrtf(2*(TERRAIN_SIZE*TERRAIN_SIZE));
        float c = ((double) rand() / RAND_MAX) * d - d/2.0;
        
        float displacement = 0.8;
        
        //iterate over all points in heightmap
        for (int x = 0; x < TERRAIN_SIZE; x++) {
            for (int z = 0; z < TERRAIN_SIZE; z++) {
                
                //increase the height
                if (a*x + b*z - c > 0)
                    heightMap[x][z] = heightMap[x][z]+displacement < 40 ? heightMap[x][z] += displacement : 40;
                
                //decrease the height
                else
                    heightMap[x][z] = heightMap[x][z]-displacement > 0 ? heightMap[x][z] -= displacement : 0;
                
            }
            
        }
        displacement += 0.01;
    }}

void Terrain::drawTerrain() {

    for (int x = 0; x < TERRAIN_SIZE-1; x++) {
        for (int z = 0; z < TERRAIN_SIZE-1; z++) {
            glBegin(GL_TRIANGLES);
                glColor3f(heightMap[x][z]/40, heightMap[x][z]/40, heightMap[x][z]/40);
                glVertex3f(x, heightMap[x][z], z);
                glColor3f(heightMap[x+1][z]/40, heightMap[x+1][z]/40, heightMap[x+1][z]/40);
                glVertex3f(x+1, heightMap[x+1][z], z);
                glColor3f(heightMap[x+1][z+1]/40, heightMap[x+1][z+1]/40, heightMap[x+1][z+1]/40);
                glVertex3f(x+1, heightMap[x+1][z+1], z+1);
            glEnd();
            
            glBegin(GL_TRIANGLES);
                glColor3f(heightMap[x][z]/40, heightMap[x][z]/40, heightMap[x][z]/40);
                glVertex3f(x, heightMap[x][z], z);
                glColor3f(heightMap[x+1][z]/40, heightMap[x+1][z]/40, heightMap[x+1][z]/40);
                glVertex3f(x+1, heightMap[x+1][z+1], z+1);
                glColor3f(heightMap[x][z+1]/40, heightMap[x][z+1]/40, heightMap[x][z+1]/40);
                glVertex3f(x, heightMap[x][z+1], z+1);
            glEnd();
            
        }
    }
}

float Terrain::getHeight(float x, float z) {
    int xInt = (int) x;
    int zInt = (int) z;
    return heightMap[xInt][zInt];
}

float Terrain::getColour(int polygonIndex) {
    return(1);
}


void Terrain::setColour(int polygonIndex, float position[3]) {
    
}