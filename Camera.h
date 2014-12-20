// CS 3GC3 Final Project
//
// Camera.h
// -interface for Camera class




class Camera {
    
public:
    
    
    /****************************************
     *    CUSTOM STRUCTS
     ****************************************/
    typedef enum {LEFT,RIGHT,FORWARD,BACK} Direction;
    
    /****************************************
     *    PUBLIC GLOBAL VARIABLES
     ****************************************/
    float position[3] = {0,0,0};
    float rotation[3] = {0,0,0};
    
    /****************************************
     *    PUBLIC FUNCTIONS
     ****************************************/
    Camera();
    void strafe(Direction, bool);
    void mouseMoved(int deltaX, int deltaY);
    int midWinX = 0;
    int midWinY = 0;
    
private:
    
    /****************************************
     *    PRIVATE GLOBAL VARIABLES
     ****************************************/
    #define TO_RADS 3.141592654/180.0
    float movementSpeedFactor = 0.2;
    float mouseSensitivity = 0.2;

};