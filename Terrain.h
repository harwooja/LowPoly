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
    
    enum Material {WATER, LAND, VOLCANO, LAVA};
    
    float getHeight(float x, float z);
    float getColour(int polygonIndex);
    void setColour(int polygonIndex, float position[3]);
    void generateTerrain();
    void drawTerrain();
    
private:

    
};

#endif
