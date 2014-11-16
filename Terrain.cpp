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



Terrain::Terrain(){
    
    
    
}


enum Material{
   
    
};


float Terrain::getHeight(float x, float z){
    
    
    return(1);
}



float Terrain::getColour(int polygonIndex){
    
    
    return(1);
}


void Terrain::setColour(int polygonIndex, float position[3]){
    
    
}