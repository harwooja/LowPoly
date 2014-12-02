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
    void strafe(Direction);
    void mouseMoved(int deltaX, int deltaY);

    
private:
    
    /****************************************
    *    PRIVATE GLOBAL VARIABLES
    ****************************************/
    #define TO_RADS 3.141592654/180.0
    float movementSpeedFactor = 0.5;
    float mouseSensitivity = 0.3;
};