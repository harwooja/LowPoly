// Terrain.h
// LowPoly
//
// Created by Jake Harwood on 2014-11-16.
// Copyright (c) 2014 Jake Harwood. All rights reserved.

class Terrain {

public:
    
    /***************************************
     *    CUSTOM STRUCTS/ENUMS
     **************************************/
    enum Material {WATER, LAND, VOLCANO, LAVA};
    
    
    /***************************************
     *    FUNCTION DECLARATIONS
     **************************************/
    Terrain();

    void generateTerrain();
    void smoothTerrain(float);
    void drawTerrain();
    void calculateFaceNormals();

    float getHeight(float x, float z);

    
    /***************************************
     *    PUBLIC GLOBAL VARIABLES
     **************************************/

};
