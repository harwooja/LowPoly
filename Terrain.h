//
//  Terrain.h
//  LowPoly
//
//  Created by Jake Harwood on 2014-11-16.
//  Copyright (c) 2014 Jake Harwood. All rights reserved.
//

#ifndef __LowPoly__Terrain__
#define __LowPoly__Terrain__

#include <stdio.h>

class Terrain{

public:
    Terrain();
    float getHeight(float x, float z);
    float getColour(int polygonIndex);
    void setColour(int polygonIndex, float position[3]);
    enum Material {Water, Land, Volcano, Lava};
    
private:
    
    
   // float vertices[x][x];
    //float colours[][];
   // float faceNormals[x][x][3];
   // float vertexNormals[x][x][3];
   // float Material[x][x];
    
    
};







#endif /* defined(__LowPoly__Terrain__) */
