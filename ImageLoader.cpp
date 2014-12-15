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
#include <string.h>
#include <stdio.h>

#include "ImageLoader.h"


/***************************************
* returns a glubyte array with RGB values
* loaded from ppm file located at filename.
* pathRelative is true if the file is
* in the same directory as the executable,
* width and height should be addresses.
**************************************/
GLubyte* ImageLoader::loadPPM(char* fileName, bool pathRelative, int* width, int* height) {
    
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
float** ImageLoader::loadPPMHeightmap(char* fileName, bool pathRelative, int terrainSize) {
    
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

//GLuint loadBMP_custom(const char* imagePath) {
//    // Data read from the header of the BMP file
//    unsigned char header[54]; // Each BMP file begins by a 54-bytes header
//    unsigned int dataPos;     // Position in the file where the actual data begins
//    unsigned int width, height;
//    unsigned int imageSize;   // = width*height*3
//    // Actual RGB data
//    unsigned char * data;
//    
//    // Open the file
//    FILE * file = fopen(imagePath,"rb");
//    if (!file) {
//        printf("Image could not be opened\n");
//        return 0;
//    }
//    
//    if (fread(header, 1, 54, file)!=54 ){ // If not 54 bytes read : problem
//        printf("Not a correct BMP file\n");
//        return false;
//    }
//
//    if ( header[0]!='B' || header[1]!='M' ){
//        printf("Not a correct BMP file\n");
//        return 0;
//    }
//    // Read ints from the byte array
//    dataPos    = *(int*)&(header[0x0A]);
//    imageSize  = *(int*)&(header[0x22]);
//    width      = *(int*)&(header[0x12]);
//    height     = *(int*)&(header[0x16]);
//    
//    // Some BMP files are misformatted, guess missing information
//    if (imageSize==0)    imageSize=width*height*3; // 3 : one byte for each Red, Green and Blue component
//    if (dataPos==0)      dataPos=54; // The BMP header is done that w
//    
//    // Create a buffer
//    data = new unsigned char [imageSize];
//    
//    // Read the actual data from the file into the buffer
//    fread(data,1,imageSize,file);
//    
//    //Everything is in memory now, the file can be closed
//    fclose(file);
//}

/***************************************
* returns current directory of running
* executable concatenated with fileName.
**************************************/
char* ImageLoader::getCurrentDirectoryPath(char* fileName) {

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
