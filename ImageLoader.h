// CS 3GC3 Final Project
//
// ImageLoader.h
// -interface for ImageLoader class

class ImageLoader {
   
    /***************************************
    *    PUBLIC FUNCTIONS
    **************************************/
    public:
    
        GLubyte* loadPPM(char* fileName, bool pathRelative, int* width, int* height);
        float** loadPPMHeightmap(char* fileName, bool pathRelative, int terrainSize);
        float** convertImageToHeightmap(GLubyte* image, int heightmapRadius);

    
    /***************************************
    *    PRIVATE FUNCTIONS
    **************************************/
    private:
 
        char* getCurrentDirectoryPath(char* fileName);

};