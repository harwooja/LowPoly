// CS 3GC3 Final Project
//
// Terrain.h
// -interface for Terrain class

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
    #define TERRAIN_SIZE 256
    #define WATER_WIDTH 4
    
    
private:
    
    /***************************************
     *    FUNCTION DECLARATIONS
     **************************************/
    void generateWater(float);
    void smoothTerrain(float);
    void calculateFaceNormals();
};
