// CS 3GC3 Final
//
// Model.h
// -public interface for Model class

class Model {

public:

    /***************************************
    *    PUBLIC FUNCTIONS
    **************************************/
    Model(char* fileName, bool pathRelative, float scaleFactor, float x, float y, float z, float* modelColour);
    void drawModel();


    /***************************************
    *    PUBLIC VARIABLES
    **************************************/
    std::vector<std::vector<float> > vertices;
    std::vector<std::vector<float> > normals;
    std::vector<std::vector<int> > faceIndices;
    std::vector<std::vector<int> > normalIndices;

    float colour[4] = {1,1,1, 1};
    float scale = 1;

    float xPos = 0;
    float yPos = 0;
    float zPos = 0;

    float rotation = 0;
    float xRot = 0;
    float yRot = 0;
    float zRot = 0;
};
