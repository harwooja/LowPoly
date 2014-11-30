//
//  Terrain.cpp
//  LowPoly
//
//  Created by Jake Harwood on 2014-11-16.
//  Copyright (c) 2014 Jake Harwood. All rights reserved.
//

//** Stores all vertices of terrain & volcano, vertex orderings, normals, and colours **//

#include "Terrain.h"
#include <vector>
#include <stdlib.h>
#include <math.h>

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

#define TERRAIN_SIZE 150

float triangles[2][3][3];
float heightMap[TERRAIN_SIZE][TERRAIN_SIZE];
float vertexNormals[TERRAIN_SIZE][TERRAIN_SIZE][3];
const float MAX_HEIGHT = 40;

Terrain::Terrain(){
    generateTerrain();
}

void Terrain::generateTerrain() {
    
    //reset heightmap
    for (int x = 0; x < TERRAIN_SIZE; x++) {
        for (int z = 0; z < TERRAIN_SIZE; z++) {
            heightMap[x][z] = 10;
        }
    }
    
    
    int iterations = (TERRAIN_SIZE*TERRAIN_SIZE*TERRAIN_SIZE)/10000+200, v;
    float displacement = 1.2, a, b, c, d;
    
    for (int i = 0; i < iterations; i++) {
        
        //choose random line
        v = rand();
        a = sinf(v);
        b = cosf(v);
        d = sqrtf(2.0*(TERRAIN_SIZE*TERRAIN_SIZE));
        c = ((double) rand()/RAND_MAX) * d - d/2.0;
        
        //iterate over all points in heightmap
        for (int x = 0; x < TERRAIN_SIZE; x++) {
            for (int z = 0; z < TERRAIN_SIZE; z++) {
                
                //increase the height
                if (a*x + b*z - c < 0) {
                    heightMap[x][z] = heightMap[x][z]+displacement < MAX_HEIGHT ? heightMap[x][z] += displacement : MAX_HEIGHT;
                }
                
                //decrease the height
                else
                    heightMap[x][z] = heightMap[x][z]-displacement > 0 ? heightMap[x][z] -= displacement : 0;
            }
            
        }
        displacement = displacement > 0.2 ? displacement-0.001 : 0.2;
    }
    
    
    smoothTerrain(TERRAIN_SIZE/600.0+0.1);
    calculateVertexNormals();

}

/*****************************************
 * Smooths out the rough terrain using
 * algorithm from:
 * www.lighthouse3d.com/opengl/terrain/index.php3?smoothing
 ****************************************/
void Terrain::smoothTerrain(float smooth) {
    
    if (smooth < 0)
        smooth = 0;
    if (smooth > 0.9)
        smooth = 0.9;
    
    //rows, left to right
    for (int x = 1; x < TERRAIN_SIZE; x++)
        for (int z = 0; z < TERRAIN_SIZE; z++)
            heightMap[x][z] = heightMap[x-1][z]*smooth + heightMap[x][z]*(1-smooth);
    
    //rows, right to left
    for (int x = TERRAIN_SIZE-2; x > -1; x--)
        for (int z = 0; z < TERRAIN_SIZE; z++)
            heightMap[x][z] = heightMap[x+1][z]*smooth + heightMap[x][z]*(1-smooth);
    
    //columns, bottom to top
    for (int x = 0; x < TERRAIN_SIZE; x++)
        for (int z = 1; z < TERRAIN_SIZE; z++)
            heightMap[x][z] = heightMap[x][z-1]*smooth + heightMap[x][z]*(1-smooth);
    
    //columns, top to bottom
    for (int x = 0; x < TERRAIN_SIZE; x++)
        for (int z = TERRAIN_SIZE-2; z > -1; z--)
            heightMap[x][z] = heightMap[x][z+1]*smooth + heightMap[x][z]*(1-smooth);
    
}

void Terrain::drawTerrain() {
//
//    for (int x = 0; x < TERRAIN_SIZE-1; x++) {
//        for (int z = 0; z < TERRAIN_SIZE-1; z++) {
//            glBegin(GL_TRIANGLES);
//                glColor3f(heightMap[x][z]/40, heightMap[x][z]/40, heightMap[x][z]/40);
//                glVertex3f(x, heightMap[x][z], z);
//                glColor3f(heightMap[x+1][z]/40, heightMap[x+1][z]/40, heightMap[x+1][z]/40);
//                glVertex3f(x+1, heightMap[x+1][z], z);
//                glColor3f(heightMap[x+1][z+1]/40, heightMap[x+1][z+1]/40, heightMap[x+1][z+1]/40);
//                glVertex3f(x+1, heightMap[x+1][z+1], z+1);
//            glEnd();
//            
//            glBegin(GL_TRIANGLES);
//                glColor3f(heightMap[x][z]/40, heightMap[x][z]/40, heightMap[x][z]/40);
//                glVertex3f(x, heightMap[x][z], z);
//                glColor3f(heightMap[x+1][z]/40, heightMap[x+1][z]/40, heightMap[x+1][z]/40);
//                glVertex3f(x+1, heightMap[x+1][z+1], z+1);
//                glColor3f(heightMap[x][z+1]/40, heightMap[x][z+1]/40, heightMap[x][z+1]/40);
//                glVertex3f(x, heightMap[x][z+1], z+1);
//            glEnd();
//            
//        }
//    }
    
    float specular[4] = {0.1,0.1,0.1, 0.5};
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 2);
    
    float terrainOffset = TERRAIN_SIZE/2.0;
    
    //iterate over all values in heightmap
    for (int x = 0; x < TERRAIN_SIZE-1; x++) {
        for (int z = 0; z < TERRAIN_SIZE-1; z++) {
            
            //set snow and water materials
            if (heightMap[x][z] <= 4){
                float diffuseWater[4] = {0,0,0.8, 1};
                float ambientWater[4] = {0,0,0.8, 1};
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientWater);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseWater);
            }
            else if (heightMap[x][z] > 4 && heightMap[x][z] <= 8 ) {
                float diffuseGrass[4] = {0,0.3,0.2, 1};
                float ambientGrass[4] = {0, 0.3, 0.2, 1};
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientGrass);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseGrass);
            }
            else if (heightMap[x][z] >= 21 ) {
                float diffuseSnow[4] = {1,1,1, 1};
                float ambientSnow[4] = {1,1,1, 1};
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientSnow);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseSnow);
            }
            else {
                float diffuseDefault[4] = {0.52,0.26,0.08, 1.0};
                float ambientDefault[4] = {0.52,0.26,0.08, 1.0};
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientDefault);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseDefault);
            }
            
            //draw the quad
            glBegin(GL_QUADS);
            glNormal3fv(vertexNormals[x][z]);
            glVertex3f(x-terrainOffset, heightMap[x][z], z-terrainOffset);

            glNormal3fv(vertexNormals[x][z+1]);
            glVertex3f(x-terrainOffset, heightMap[x][z+1], z+1-terrainOffset);
            
            glNormal3fv(vertexNormals[x+1][z+1]);
            glVertex3f(x+1-terrainOffset, heightMap[x+1][z+1], z+1-terrainOffset);

            glNormal3fv(vertexNormals[x+1][z]);
            glVertex3f(x+1-terrainOffset, heightMap[x+1][z], z-terrainOffset);
            glEnd();
        }
    }
}

/**************************************************************
 * calculates normals for every vertex in the heightmap.
 * Code was taken from:
 * www.lighthouse3d.com/opengl/terrain/index.php3?normals
 * We modified the code to suit our needs.
 **************************************************************/
void Terrain::calculateVertexNormals() {
    
    //calculate normals
    for (int x = 0; x < TERRAIN_SIZE-1; x++) {
        for (int z = 0; z < TERRAIN_SIZE-1; z++) {
            
            //x, z
            float t1[3];
            t1[0] = x; t1[1] = heightMap[x][z]; t1[2] = z;
            
            //x+1, z
            float t3[3];
            t3[0] = x+1; t3[1] = heightMap[x+1][z]; t3[2] = z;
            
            //x, z+1
            float t2[3];
            t2[0] = x; t2[1] = heightMap[x][z+1]; t2[2] = z+1;
            
            float v1[3] = {t2[0]-t1[0], t2[1]-t1[1], t2[2]-t1[2]};
            float v2[3] = {t3[0]-t1[0], t3[1]-t1[1], t3[2]-t1[2]};
            
            float vx = v1[1]*v2[2] - v1[2]*v2[1];
            float vy = v1[2]*v2[0] - v1[0]*v2[2];
            float vz = v1[0]*v2[1] - v1[1]*v2[0];
            
            float len = sqrtf(vx*vx + vy*vy + vz*vz);
            float nv[3] = {vx/len, vy/len, vz/len};
            
            vertexNormals[x][z][0] = nv[0];
            vertexNormals[x][z][1] = nv[1];
            vertexNormals[x][z][2] = nv[2];
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