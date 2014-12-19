// CS 3GC3 Final Project
//
// Terrain.cpp
// -stores all vertices and colours
// -uses heightmap to generate the terrain
// -has functions to draw terrain and get
// important information about it

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

#include <stdlib.h>
#include <math.h>

#include "Terrain.h"
#include "ResourceLoader.h"
#include "Model.h"

/***************************************
 *    GLOBAL VARIABLES
 **************************************/
float heightMap[TERRAIN_SIZE+WATER_WIDTH][TERRAIN_SIZE+WATER_WIDTH];
float faceNormals[TERRAIN_SIZE+WATER_WIDTH][TERRAIN_SIZE+WATER_WIDTH][3];
float materialColours[TERRAIN_SIZE+WATER_WIDTH][TERRAIN_SIZE+WATER_WIDTH][4];
std::vector<Model> models;

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
    
    float terrainRadius = (TERRAIN_SIZE+WATER_WIDTH)/2.0;
    
    //reset heightmap
    for (int x = 0; x < TERRAIN_SIZE+WATER_WIDTH; x++)
        for (int z = 0; z < TERRAIN_SIZE+WATER_WIDTH; z++)
            heightMap[x][z] = 0;
    
    //load heightmap image
    ResourceLoader imgLoader = ResourceLoader();
    float** heightmapImage = imgLoader.loadPPMHeightmap((char*)"/images/heightmap_256.ppm", true, TERRAIN_SIZE);
    
    //iterate over all points in heightmap (not incl. water)
    for (int x = WATER_WIDTH; x < TERRAIN_SIZE+WATER_WIDTH-1; x++) {
        for (int z = WATER_WIDTH; z < TERRAIN_SIZE+WATER_WIDTH-1; z++) {
            
            //set height
            heightMap[x][z] = 40*heightmapImage[x-WATER_WIDTH][z-WATER_WIDTH];
            
            //set volcano pos to highest point
            if (heightMap[x][z] > volcanoPos[1]) {
                volcanoPos[0] = x-terrainRadius;
                volcanoPos[1] = heightMap[x][z];
                volcanoPos[2] = z-terrainRadius;
            }
            
            //grass
            if (heightMap[x][z] <= 15) {
                materialColours[x][z][0] = 0.0;
                materialColours[x][z][1] = 0.3;
                materialColours[x][z][2] = 0.2;
            }
            //grass to dirt transition
            else if (heightMap[x][z] > 15 && heightMap[x][z] <= 20) {
                materialColours[x][z][0] = 0.0 + ((heightMap[x][z]-20)*0.1);
                materialColours[x][z][1] = 0.3 - ((heightMap[x][z]-20)*0.01);
                materialColours[x][z][1] = 0.2 - ((heightMap[x][z]-20)*0.02);
            }
            //dirt
            else if (heightMap[x][z] > 20 && heightMap[x][z] <= 30) {
                materialColours[x][z][0] = 0.52;
                materialColours[x][z][1] = 0.26;
                materialColours[x][z][2] = 0.08;
            }
            //dirt to snow transition
            else if (heightMap[x][z] > 30 && heightMap[x][z] <= 35) {
                materialColours[x][z][0] = 0.52 + ((heightMap[x][z]-30)*0.1);
                materialColours[x][z][1] = 0.26 + ((heightMap[x][z]-30)*0.15);
                materialColours[x][z][2] = 0.08 + ((heightMap[x][z]-30)*0.18);
            }
            //snow
            else {
                materialColours[x][z][0] = 1;
                materialColours[x][z][1] = 1;
                materialColours[x][z][2] = 1;
            }
            materialColours[x][z][3] = 1;
        }
    }
    
    generateWater(3);
    smoothTerrain(0.2);
    calculateFaceNormals();
    
    //load trees and rocks
    elephantVertices.clear();
    elephantNormals.clear();
    elephantNormalIndices.clear();
    elephantFaceIndices.clear();
    //imgLoader.loadObj((char*)"/images/elephant.obj", true, &elephantVertices, &elephantNormals, &elephantFaceIndices, &elephantNormalIndices);
}

/***************************************
 * for specified waterwidth (global),
 * creates choppy white (foaming) water
 * around terrain. Modifies heightmap.
 **************************************/
void Terrain::generateWater(float choppiness) {
    
    for (int x = TERRAIN_SIZE; x < TERRAIN_SIZE+WATER_WIDTH; x++) {
        for (int z = 0; z < TERRAIN_SIZE+WATER_WIDTH; z++) {
            float a = rand();
            heightMap[x][z] = choppiness*cosf(a);
            materialColours[x][z][0] = 0.7;
            materialColours[x][z][1] = 0.7;
            materialColours[x][z][2] = 0.8;
            materialColours[x][z][3] = 1;
        }
    }
    
    for (int x = 0; x < WATER_WIDTH; x++) {
        for (int z = 0; z < TERRAIN_SIZE+WATER_WIDTH; z++) {
            float a = rand();
            heightMap[x][z] = choppiness*cosf(a);
            materialColours[x][z][0] = 0.7;
            materialColours[x][z][1] = 0.7;
            materialColours[x][z][2] = 0.8;
            materialColours[x][z][3] = 1;
        }
    }
    
    for (int x = 0; x < TERRAIN_SIZE+WATER_WIDTH; x++) {
        for (int z = TERRAIN_SIZE; z < TERRAIN_SIZE+WATER_WIDTH; z++) {
            float a = rand();
            heightMap[x][z] = choppiness*cosf(a);
            materialColours[x][z][0] = 0.7;
            materialColours[x][z][1] = 0.7;
            materialColours[x][z][2] = 0.8;
            materialColours[x][z][3] = 1;
        }
    }
    
    for (int x = 0; x < TERRAIN_SIZE+WATER_WIDTH; x++) {
        for (int z = 0; z < WATER_WIDTH; z++) {
            float a = rand();
            heightMap[x][z] = choppiness*cosf(a);
            materialColours[x][z][0] = 0.7;
            materialColours[x][z][1] = 0.7;
            materialColours[x][z][2] = 0.8;
            materialColours[x][z][3] = 1;
        }
    }
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
* draws our elephant
**************************************************************/

/**
void drawElephant() {
    
    float colour[3] = {1,1,1};
    glMaterialfv(GL_DIFFUSE, GL_FRONT_AND_BACK, colour);
    glMaterialfv(GL_AMBIENT, GL_FRONT_AND_BACK, colour);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    glTranslatef(0, 30, -10);
    glScalef(0.01, 0.01, 0.01);

    for (int i = 0; i < elephantFaceIndices.size()-2; i++) {
        glBegin(GL_TRIANGLES);
        glNormal3f(elephantNormals[elephantNormalIndices[i][0]][0], elephantNormals[elephantNormalIndices[i][0]][1], elephantNormals[elephantNormalIndices[i][0]][2]);
        glVertex3f(elephantVertices[elephantFaceIndices[i][0]][0], elephantVertices[elephantFaceIndices[i][0]][1], elephantVertices[elephantFaceIndices[i][0]][2]);
        
        glNormal3f(elephantNormals[elephantNormalIndices[i][1]][0], elephantNormals[elephantNormalIndices[i][1]][1], elephantNormals[elephantNormalIndices[i][1]][2]);
        glVertex3f(elephantVertices[elephantFaceIndices[i][1]][0], elephantVertices[elephantFaceIndices[i][1]][1], elephantVertices[elephantFaceIndices[i][1]][2]);
        
        glNormal3f(elephantNormals[elephantNormalIndices[i][2]][0], elephantNormals[elephantNormalIndices[i][2]][1], elephantNormals[elephantNormalIndices[i][2]][2]);
        glVertex3f(elephantVertices[elephantFaceIndices[i][2]][0], elephantVertices[elephantFaceIndices[i][2]][1], elephantVertices[elephantFaceIndices[i][2]][2]);
        glEnd();
    }
    
    glPopMatrix();
}
 **/


/**************************************************************
 * draws our generated terrain
 **************************************************************/
void Terrain::drawTerrain() {
    
    float terrainOffset = (TERRAIN_SIZE+WATER_WIDTH)/2.0;

    //iterate over all values in heightmap
    for (int x = 0; x < TERRAIN_SIZE+WATER_WIDTH-1; x++) {
        glBegin(GL_QUAD_STRIP);
        for (int z = 0; z < TERRAIN_SIZE+WATER_WIDTH; z++) {
            
            glMaterialfv(GL_FRONT, GL_AMBIENT, materialColours[x][z]);
            glMaterialfv(GL_FRONT, GL_DIFFUSE, materialColours[x][z]);

            glNormal3fv(faceNormals[x+1][z]);
            glVertex3f(x+1-terrainOffset, heightMap[x+1][z], z-terrainOffset);
            
            glNormal3fv(faceNormals[x][z]);
            glVertex3f(x-terrainOffset, heightMap[x][z], z-terrainOffset);
            
        }
        glEnd();
    }
    
    //draw water
    float waterColour[4] = {0.1, 0.6, 1, 1};
    glMaterialfv(GL_FRONT, GL_AMBIENT, waterColour);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, waterColour);
    
    glPushMatrix();
    glScalef(100, 1, 100);
    glTranslatef(0, -2, 0);
    glutSolidCube(10);
    glPopMatrix();
    
    //draw elephant
   // drawElephant();
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
 * darkens quad at x,z
 **************************************************************/
void Terrain::burnTerrain(float x, float z) {
    
    //coordinate (0,0) corresponds with middle of terrain map
    int xIndex = x + (TERRAIN_SIZE+WATER_WIDTH)/2.0;
    int zIndex = z + (TERRAIN_SIZE+WATER_WIDTH)/2.0 + 1;
    
    //if outside of terrain, ignore
    if (xIndex < 0 || xIndex >= TERRAIN_SIZE+WATER_WIDTH-2)
        return;
    if (zIndex < 0 || zIndex >= TERRAIN_SIZE+WATER_WIDTH-2)
        return;
    
    //darken square ("charred")
    materialColours[xIndex][zIndex][0] -= 0.1;
    materialColours[xIndex][zIndex][1] -= 0.1;
    materialColours[xIndex][zIndex][2] -= 0.1;
}

/**************************************************************
 * makes terrain at x,z whiter
 **************************************************************/
void Terrain::snowTerrain(float x, float z) {
    
    //coordinate (0,0) corresponds with middle of terrain map
    int xIndex = x + (TERRAIN_SIZE+WATER_WIDTH)/2.0;
    int zIndex = z + (TERRAIN_SIZE+WATER_WIDTH)/2.0 + 1;
    
    //if outside of terrain, ignore
    if (xIndex < 0 || xIndex >= TERRAIN_SIZE+WATER_WIDTH-2)
        return;
    if (zIndex < 0 || zIndex >= TERRAIN_SIZE+WATER_WIDTH-2)
        return;
    
    //lighten square ("snow")
    materialColours[xIndex][zIndex][0] += 0.3;
    materialColours[xIndex][zIndex][1] += 0.3;
    materialColours[xIndex][zIndex][2] += 0.3;
    
}

/**************************************************************
 * returns height of terrain at x,z using bilinear interpolation
 **************************************************************/
float Terrain::getHeight(float x, float z) {
    
    //coordinate (0,0) corresponds with middle of terrain map
    float xIndexInHeightmap = x + (TERRAIN_SIZE+WATER_WIDTH)/2.0;
    float zIndexInHeightmap = z + (TERRAIN_SIZE+WATER_WIDTH)/2.0;
    
    //if outside of terrain, set height to 0
    if (xIndexInHeightmap < 0 || xIndexInHeightmap >= TERRAIN_SIZE+WATER_WIDTH-2)
        return 3;
    if (zIndexInHeightmap < 0 || zIndexInHeightmap >= TERRAIN_SIZE+WATER_WIDTH-2)
        return 3;
    if (heightMap[(int)floor(xIndexInHeightmap)][(int)floor(zIndexInHeightmap)] < 1)
        return 1;
    
    // B(0,1) ------ C(1,1)
    //   |      pos    |
    // A(0,0) ------ D(1,0)
    float A = heightMap[(int)floor(xIndexInHeightmap)][(int)floor(zIndexInHeightmap)];
    float B = heightMap[(int)floor(xIndexInHeightmap)][(int)floor(zIndexInHeightmap+1)];
    float C = heightMap[(int)floor(xIndexInHeightmap+1)][(int)floor(zIndexInHeightmap+1)];
    float D = heightMap[(int)floor(xIndexInHeightmap+1)][(int)floor(zIndexInHeightmap)];
    
    //calculate percent position is along x and z, in [0,1]
    float xPercent = xIndexInHeightmap-floor(xIndexInHeightmap);
    float zPercent = zIndexInHeightmap-floor(zIndexInHeightmap);
    
    //calculate height, interpolating between 4 points surrounding current pos
    //this formula is from wikipedia.com/Bilinear_interpolation
    float h = A*(1-xPercent)*(1-zPercent) + D*(xPercent)*(1-zPercent);
    h += B*(1-xPercent)*(zPercent)+C*(xPercent)*(zPercent);
    
    return h;
}

/**************************************************************
 * returns face normal at point x,z
 **************************************************************/
float* Terrain::getNormal(float x, float z) {
    
    //coordinate (0,0) corresponds with middle of terrain map
    int xIndex = x + (TERRAIN_SIZE+WATER_WIDTH)/2.0;
    int zIndex = z + (TERRAIN_SIZE+WATER_WIDTH)/2.0;
    
    //if outside of terrain, return null
    if (xIndex < 0 || xIndex >= TERRAIN_SIZE+WATER_WIDTH-2)
        return NULL;
    if (zIndex < 0 || zIndex >= TERRAIN_SIZE+WATER_WIDTH-2)
        return NULL;
    
    return faceNormals[xIndex][zIndex];
}