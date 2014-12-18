// CS 3GC3 Final Project
//
// ResourceLoader.h
// -interface for ResourceLoader class

#include <vector>

class ResourceLoader {
   
    /***************************************
    *    PUBLIC FUNCTIONS
    **************************************/
    public:
    
        GLubyte* loadPPM(char* fileName, bool pathRelative, int* width, int* height);
        void loadPPMTexture(char* fileName, bool pathRelative, GLuint* textures);
        float** loadPPMHeightmap(char* fileName, bool pathRelative, int terrainSize);
        float** convertImageToHeightmap(GLubyte* image, int heightmapRadius);
        void loadObj(char *fileName, bool pathRelative, std::vector<std::vector<float>>* vertices, std::vector<std::vector<float>>* normals, std::vector<std::vector<int>>* faceIndices, std::vector<std::vector<int>>* normalIndices);


};