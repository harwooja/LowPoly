//
//  ObjLoader.cpp
//  LowPoly
//
//  Created by Jake Harwood on 2014-12-13.
//  Copyright (c) 2014 Stuart Douglas. All rights reserved.
//
// Citation: http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/


#ifdef __APPLE__
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <GLUT/glut.h>
#else
#ifdef _WIN32
# include <windows.h>
#endif
# include <GL/gl.h>
# include <GL/glu.h>
# include <GL/freeglut.h>
#endif



#include "ObjLoader.h"
#include <string>

using namespace std;
using namespace glm;


vector< unsigned int > vertexIndices, uvIndices, normalIndices;
vector< glm::vec3 > temp_vertices;
vector< glm::vec2 > temp_uvs;
vector< glm::vec3 > temp_normals;

// Read our .obj file
std::vector< glm::vec3 > vertices;
std::vector< glm::vec2 > uvs;
std::vector< glm::vec3 > normals; // Won't be used at the moment.


ObjLoader::ObjLoader(){
    

    loadOBJ("/Users/Harwood/Documents/GitHub/LowPoly/Pikachu.Obj", vertices, uvs, normals);


    
}

void ObjLoader::render(){
    
   glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    
  

}

void ObjLoader::loadOBJ(const char * path, vector < glm::vec3 > & out_vertices, vector < glm::vec2 > &out_uvs, vector < glm::vec3 > &out_normals){
    
    FILE* file = fopen(path, "r");
    
    if(file == NULL){
        printf("Impossible to open the file !\n");
        exit(0);
    }
    
    while(true){
        
        char lineHeader[128];
        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.
        
        // else : parse lineHeader
        
        
        if ( strcmp( lineHeader, "v" ) == 0 ){
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
            temp_vertices.push_back(vertex);
            
            
        }else if ( strcmp( lineHeader, "vt" ) == 0 ){
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            temp_uvs.push_back(uv);
            
        }else if ( strcmp( lineHeader, "vn" ) == 0 ){
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z );
            temp_normals.push_back(normal);
            
        }else if ( strcmp( lineHeader, "f" ) == 0 ){
            string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
            
            
            if (matches != 9){
                printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                exit(0);
            }
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices    .push_back(uvIndex[0]);
            uvIndices    .push_back(uvIndex[1]);
            uvIndices    .push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
            
        }
        
        
    }
    
    // For each vertex of each triangle
    for( unsigned int i=0; i<vertexIndices.size(); i++ ){
        unsigned int vertexIndex = vertexIndices[i];
        vec3 vertex = temp_vertices[ vertexIndex-1 ];
        out_vertices.push_back(vertex);
        
    }
    
    
}








