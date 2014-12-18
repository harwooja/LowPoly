// CS 3GC3 Final Project
//
// ImageLoader.cpp
// -loads PPM files from specified paths and
// returns them
// -has function to load a heightmap PPM file
// which returns 2D float array


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
#include <unistd.h>
#include <string>
#include <stdio.h>

#include "ResourceLoader.h"

/***************************************
 * FUCNTION PROTOTYPES
 **************************************/
char* getCurrentDirectoryPath(char* fileName);

/***************************************
* returns a glubyte array with RGB values
* loaded from ppm file located at filename.
* pathRelative is true if the file is
* in the same directory as the executable,
* width and height should be addresses.
**************************************/
GLubyte* ResourceLoader::loadPPM(char* fileName, bool pathRelative, int* width, int* height) {
    
    //if path is relative to running directory, get absolute path
    if (pathRelative)
        fileName = getCurrentDirectoryPath(fileName);
    
    //open the file in read mode
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Error. File \"%s\" could not be loaded.",fileName);
        exit(0);
    }
    
    //scan everything up to new line
    char b[100];
    fscanf(file,"%[^\n] ", b);
    
    //check if first two characters are not P3. If not, it's not an ASCII PPM file
    if (b[0]!='P'|| b[1] != '3') {
        printf("%s is not a PPM file!\n",fileName);
        exit(0);
    }
    
    //read past the file comments (then go back 1 so we don't miss the size)
    char c;
    fscanf(file, "%c", &c);
    while(c == '#')	{
        fscanf(file, "%[^\n] ", b);
        fscanf(file, "%c",&c);
    }
    ungetc(c,file);
    
    //read the rows, columns and max colour values
    int maxColour, imageWidth, imageHeight;
    fscanf(file, "%d %d %d", &imageWidth, &imageHeight, &maxColour);
    
    //number of pixels is width * height
    int size = imageWidth*imageHeight;
    
    //allocate memory to store 3 GLuints for every pixel
    GLubyte* image = (GLubyte*) malloc(3*sizeof(GLuint)*size);
    
    //scale the colour in case maxCol is not 255
    float scaledColour = 255.0/maxColour;
    
    //start reading pixel colour data
    int red, green, blue;
    for (int i = 0; i < size; i++) {
        fscanf(file,"%d %d %d",&red, &green, &blue );
        image[3*size-3*i-3] = red*scaledColour;
        image[3*size-3*i-2] = green*scaledColour;
        image[3*size-3*i-1] = blue*scaledColour;
    }
    
    *width = imageWidth;
    *height = imageHeight;
    
    return image;
}

/***************************************
* returns a 2d array of floats on [0,1].
* 0 is black, 1 is white. Exits on error.
***************************************/
float** ResourceLoader::loadPPMHeightmap(char* fileName, bool pathRelative, int terrainSize) {
    
    //load the image
    int width, height;
    GLubyte* img = loadPPM(fileName, pathRelative, &width, &height);
    
    //check that image is same size as terrain
    if (width < terrainSize || height < terrainSize) {
        printf("\nError. Image too small.");
        exit(0);
    }
    else if (width > terrainSize || height > terrainSize) {
        printf("\nError. Image too large.");
        exit(0);
    }
    
    //allocate mem for our resulting image
    float **heightmapImage = new float*[terrainSize];
    for (int i = 0; i < terrainSize; i++)
        heightmapImage[i] = new float[terrainSize];
    
    //convert image to 2D float array, averaging RGB values
    for (int i = 0; i < terrainSize; i++) {
        for (int j = 0; j < terrainSize; j++) {
            int subscript = (int) (3*(i*terrainSize+j) + 3*(i*terrainSize+j)+1 + 3*(i*terrainSize+j)+2)/3.0;
            heightmapImage[i][j] = img[subscript]/255.0;
        }
    }
    
    return heightmapImage;
}

/******************************************
* loads the image into the textures array
* element passed in, setting all parameters
******************************************/
void ResourceLoader::loadPPMTexture(char* fileName, bool pathRelative, GLuint* textures) {

    //load the image from the file
    int width, height;
    GLubyte* img = loadPPM(fileName, pathRelative, &width, &height);
    
    //set texture properties
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
}

/***************************************
* loads a .obj file into passed variables
**************************************/
void ResourceLoader::loadObj(char *fileName, bool pathRelative, std::vector<std::vector<float>>* vertices, std::vector<std::vector<float>>* normals, std::vector<std::vector<int>>* faceIndices, std::vector<std::vector<int>>* normalIndices) {
    
    //if path is relative to running directory, get absolute path
    if (pathRelative)
        fileName = getCurrentDirectoryPath(fileName);
    
    FILE *objectFile;
    float x, y, z;
    
    //read file
    objectFile = fopen(fileName, "r");
    if (objectFile == NULL) {
        printf("Error. File \"%s\" could not be loaded.",fileName);
        exit(0);
    }
    
    //set up variables for reading data
    std::vector<float> vertexTemp;
    std::vector<float> normalTemp;
    std::vector<int> indexTemp;
    std::string vertex1, vertex2, vertex3;
    
    char line[128];
    int faceIndex[3], normalIndex[3];
    int res, matches;
    float normalLength = 1;
    
    //go through each line
    while (1) {
        
        //end of file
        res = fscanf(objectFile, "%s", line);
        if (res == EOF)
            break;
        
        //vertex
        if (strcmp(line, "v") == 0) {
            fscanf(objectFile, "%f %f %f\n", &x, &y, &z);
            vertexTemp.push_back(x);
            vertexTemp.push_back(y);
            vertexTemp.push_back(z);
            vertices->push_back(vertexTemp);
            vertexTemp.clear();
        }
        //normal
        else if (strcmp(line, "vn") == 0) {
            fscanf(objectFile, "%f %f %f\n", &x, &y, &z);
            normalLength = x + y + z;
            normalTemp.push_back(x/normalLength);
            normalTemp.push_back(y/normalLength);
            normalTemp.push_back(z/normalLength);
            normals->push_back(normalTemp);
            normalTemp.clear();
        }
        //index
        else if (strcmp(line, "f") == 0) {
            
            //load the data
            matches = fscanf(objectFile, "%d//%d %d//%d %d//%d\n", &faceIndex[0], &normalIndex[0], &faceIndex[1], &normalIndex[1], &faceIndex[2], &normalIndex[2]);

            //check that .obj file is in right format
            if (matches != 6) {
                printf("Error. Object file %s must have only face and normal indices", fileName);
                exit(1);
            }
            
            indexTemp.push_back(faceIndex[0]-1);
            indexTemp.push_back(faceIndex[1]-1);
            indexTemp.push_back(faceIndex[2]-1);
            faceIndices->push_back(indexTemp);
            indexTemp.clear();
            
            indexTemp.push_back(normalIndex[0]-1);
            indexTemp.push_back(normalIndex[1]-1);
            indexTemp.push_back(normalIndex[2]-1);
            normalIndices->push_back(indexTemp);
            indexTemp.clear();
        }
        
    }
    fclose(objectFile);
}

/***************************************
* returns current directory of running
* executable concatenated with fileName.
**************************************/
char* getCurrentDirectoryPath(char* fileName) {

    //initialize path
    char currentDirPath[1024];
    
    //if path not too long, return it concatenated with fileName
    if (getcwd(currentDirPath, sizeof(currentDirPath)) != NULL) {
        
        //create path
        char* filePath = (char*) malloc(strlen(currentDirPath)+strlen(fileName)+1);
        strcpy(filePath, currentDirPath);
        strcat(filePath, fileName);

        return filePath;
    }
    
    //print error message and exit
    else {
        printf("\nError. Could not get current directory path.");
        exit(0);
    }
}
