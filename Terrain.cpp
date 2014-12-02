// Terrain.cpp
// LowPoly
//
// Created by Jake Harwood on 2014-11-16.
// Copyright (c) 2014 Jake Harwood. All rights reserved.

//** Stores all vertices of terrain & volcano, vertex orderings, normals, and colours **//

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

#include "Terrain.h"
#include <vector>
#include <stdlib.h>
#include <math.h>


/***************************************
 *    GLOBAL VARIABLES
 **************************************/
#define TERRAIN_SIZE 100
#define WATER_WIDTH 20

float heightMap[TERRAIN_SIZE+WATER_WIDTH][TERRAIN_SIZE+WATER_WIDTH];
float faceNormals[TERRAIN_SIZE+WATER_WIDTH][TERRAIN_SIZE+WATER_WIDTH][3];
const float MAX_HEIGHT = 40;

/***************************************
 * Constructor
 **************************************/
Terrain::Terrain(){
    generateTerrain();
}

/***************************************
* uses fault line algorithm to create
* the terrain, stored in heightMap
**************************************/
void Terrain::generateTerrain() {
    
    //reset heightmap
    for (int x = 0; x < TERRAIN_SIZE+WATER_WIDTH; x++)
        for (int z = 0; z < TERRAIN_SIZE+WATER_WIDTH; z++)
            heightMap[x][z] = 0;
    
    int v;
    float displacement = 1.2, a, b, c, d;
    
    for (int i = 0; i < 550; i++) {
        
        //choose random line
        v = rand();
        a = sinf(v);
        b = cosf(v);
        d = sqrtf(2.0*(TERRAIN_SIZE*TERRAIN_SIZE));
        c = ((double) rand()/RAND_MAX) * d - d/2.0;
        
        //iterate over all points in heightmap (not incl. water)
        for (int x = WATER_WIDTH; x < TERRAIN_SIZE+WATER_WIDTH-1; x++) {
            for (int z = WATER_WIDTH; z < TERRAIN_SIZE+WATER_WIDTH-1; z++) {
                
                //increase the height
                if (a*x + b*z - c < 0)
                    heightMap[x][z] = heightMap[x][z]+displacement < MAX_HEIGHT ? heightMap[x][z] += displacement : MAX_HEIGHT;
                
                //decrease the height
                else
                    heightMap[x][z] -= displacement;
            }
            
        }
        displacement = displacement > 0.2 ? displacement-0.001 : 0.2;
    }
    
    //generate the water
    float choppiness = 3;
    for (int x = TERRAIN_SIZE; x < TERRAIN_SIZE+WATER_WIDTH; x++) {
        for (int z = 0; z < TERRAIN_SIZE+WATER_WIDTH; z++) {
            a = rand();
            heightMap[x][z] = choppiness*cosf(a);
        }
    }
    for (int x = 0; x < WATER_WIDTH; x++) {
        for (int z = 0; z < TERRAIN_SIZE+WATER_WIDTH; z++) {
            a = rand();
            heightMap[x][z] = choppiness*cosf(a);
        }
    }
    for (int x = 0; x < TERRAIN_SIZE+WATER_WIDTH; x++) {
        for (int z = TERRAIN_SIZE; z < TERRAIN_SIZE+WATER_WIDTH; z++) {
            a = rand();
            heightMap[x][z] = choppiness*cosf(a);
        }
    }
    for (int x = 0; x < TERRAIN_SIZE+WATER_WIDTH; x++) {
        for (int z = 0; z < WATER_WIDTH; z++) {
            a = rand();
            heightMap[x][z] = choppiness*cosf(a);
        }
    }
    smoothTerrain(0.4);
    calculateFaceNormals();
    
//   int ab = (int) (TERRAIN_SIZE+WATER_WIDTH)/2.0;
//    printf("\n\nheight at          (0,0): %f\ncomputed height at (0,0): %f",heightMap[ab][ab], getHeight(0,0));
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
    for (int x = 1; x < TERRAIN_SIZE+WATER_WIDTH; x++)
        for (int z = 0; z < TERRAIN_SIZE+WATER_WIDTH; z++)
            heightMap[x][z] = heightMap[x-1][z]*smooth + heightMap[x][z]*(1-smooth);
    
    //rows, right to left
    for (int x = TERRAIN_SIZE+WATER_WIDTH-2; x > -1; x--)
        for (int z = 0; z < TERRAIN_SIZE+WATER_WIDTH; z++)
            heightMap[x][z] = heightMap[x+1][z]*smooth + heightMap[x][z]*(1-smooth);
    
    //columns, bottom to top
    for (int x = 0; x < TERRAIN_SIZE+WATER_WIDTH; x++)
        for (int z = 1; z < TERRAIN_SIZE+WATER_WIDTH; z++)
            heightMap[x][z] = heightMap[x][z-1]*smooth + heightMap[x][z]*(1-smooth);
    
    //columns, top to bottom
    for (int x = 0; x < TERRAIN_SIZE+WATER_WIDTH; x++)
        for (int z = TERRAIN_SIZE+WATER_WIDTH-2; z > -1; z--)
            heightMap[x][z] = heightMap[x][z+1]*smooth + heightMap[x][z]*(1-smooth);
    
}


/**************************************************************
* draws our generated terrain
**************************************************************/
void Terrain::drawTerrain() {

    float terrainOffset = (TERRAIN_SIZE+WATER_WIDTH)/2.0;
    
    //iterate over all values in heightmap
    for (int x = 0; x < TERRAIN_SIZE+WATER_WIDTH-1; x++) {
        for (int z = 0; z < TERRAIN_SIZE+WATER_WIDTH-1; z++) {
            
            //water
            if (x < WATER_WIDTH || z < WATER_WIDTH || x >= TERRAIN_SIZE || z >= TERRAIN_SIZE){
                float diffuseWater[4] = {0,0,0.8, 1};
                float ambientWater[4] = {0,0,0.8, 1};
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientWater);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseWater);
            }
            
            //grass
            else if (heightMap[x][z] <= 20 ) {
                float diffuseGrass[4] = {0,0.3,0.2, 1};
                float ambientGrass[4] = {0, 0.3, 0.2, 1};
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientGrass);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseGrass);
            }
            
            //snow
            else if (heightMap[x][z] >= 32 ) {
                float diffuseSnow[4] = {1,1,1, 1};
                float ambientSnow[4] = {1,1,1, 1};
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientSnow);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseSnow);
            }
            
            //dirt
            else {
                float diffuseDirt[4] = {0.52,0.26,0.08, 1.0};
                float ambientDirt[4] = {0.52,0.26,0.08, 1.0};
                glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambientDirt);
                glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuseDirt);
            }
            
            glNormal3fv(faceNormals[x][z]);
            
            //draw the quad
            glBegin(GL_QUADS);
                glVertex3f(x-terrainOffset, heightMap[x][z], z-terrainOffset);
                glVertex3f(x-terrainOffset, heightMap[x][z+1], z+1-terrainOffset);
                glVertex3f(x+1-terrainOffset, heightMap[x+1][z+1], z+1-terrainOffset);
                glVertex3f(x+1-terrainOffset, heightMap[x+1][z], z-terrainOffset);
            glEnd();
        }
    }

//    glDisable(GL_LIGHTING);
//    glColor3f(0, 0.2, 0.8);
//    glMatrixMode(GL_MODELVIEW);
//    glPushMatrix();
//    glScalef(50, 1, 50);
//    glTranslatef(0, -10, 0);
//
//    glutSolidCube(10);
//    
//    glPopMatrix();
//    glEnable(GL_LIGHTING);
}

/**************************************************************
 * calculates normals for every vertex in the heightmap.
 * Code was taken from:
 * www.lighthouse3d.com/opengl/terrain/index.php3?normals
 **************************************************************/
void Terrain::calculateFaceNormals() {
    
    //calculate normals
    for (int x = 0; x < TERRAIN_SIZE+WATER_WIDTH; x++) {
        for (int z = 0; z < TERRAIN_SIZE+WATER_WIDTH; z++) {
            
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
            
            faceNormals[x][z][0] = nv[0];
            faceNormals[x][z][1] = nv[1];
            faceNormals[x][z][2] = nv[2];
        }
    }
}

/**************************************************************
 * returns height of terrain at x,z using bilinear interpolation
 **************************************************************/
float Terrain::getHeight(float x, float z) {

    float xIndexInHeightmap = x + (TERRAIN_SIZE+WATER_WIDTH)/2.0;
    float zIndexInHeightmap = z + (TERRAIN_SIZE+WATER_WIDTH)/2.0;

    if (xIndexInHeightmap < 0 || zIndexInHeightmap < 0 || xIndexInHeightmap >= TERRAIN_SIZE+WATER_WIDTH || zIndexInHeightmap >= TERRAIN_SIZE+WATER_WIDTH)
        return 0;
    
    float aHeight = heightMap[(int)floor(xIndexInHeightmap)][(int)floor(zIndexInHeightmap)];
    float bHeight = heightMap[(int)floor(xIndexInHeightmap+1)][(int)floor(zIndexInHeightmap+1)];
    
    float xPercent = xIndexInHeightmap-floor(xIndexInHeightmap);
    float zPercent = zIndexInHeightmap-floor(zIndexInHeightmap);
    float distOnABLine = sqrtf(xPercent*xPercent + zPercent*zPercent);
    
    return aHeight+distOnABLine*(bHeight-aHeight);
}
