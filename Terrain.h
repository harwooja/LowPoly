// Terrain.h
// LowPoly
//
// Created by Jake Harwood on 2014-11-16.
// Copyright (c) 2014 Jake Harwood. All rights reserved.

class Terrain {

public:

    /***************************************
     *    FUNCTION DECLARATIONS
     **************************************/
    Terrain();

    void generateTerrain();
    void drawTerrain();
    void burnTerrain(float x, float z);
    void snowTerrain(float x, float z);

    float getHeight(float x, float z);
    float* getNormal(float x, float z);

    /***************************************
     *    PUBLIC GLOBAL VARIABLE
     **************************************/
    float volcanoPos[3] = {0,0,0};

private:

    /***************************************
    *    FUNCTION DECLARATIONS
    **************************************/
    void generateWater(float);
    void smoothTerrain(float);
    void calculateFaceNormals();
};
