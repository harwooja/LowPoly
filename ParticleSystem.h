class ParticleSystem {

    public:
    
        /****************************************
        *    CUSTOM STRUCTS
        ****************************************/
        typedef struct {
            float x = 0;
            float y = 0;
            float z = 0;
            float xDir = 0;
            float yDir = 0;
            float zDir = 0;
            float xRot = 0;
            float yRot = 0;
            float zRot = 0;
            float xRotIncr = 0;
            float yRotIncr = 0;
            float zRotIncr = 0;
            
            float red = 1.0;
            float green = 1.0;
            float blue = 1.0;
            
            float speed = 0.1;
            float sizeMultiplier = 1;
            int age = 0;
            
            bool explosion = false;
            
        } Particle;
    
        /****************************************
        *    FUNCTION DECLARATIONS
        ****************************************/

        //constructor
        ParticleSystem(Terrain*);

        void addParticle();
        void clearParticles();

        void moveParticles();
        void drawParticles();
    
        /****************************************
        *    PUBLIC GLOBAL VARIABLES
        ****************************************/
        float spawnRate;
        float particleSize;
        float emitterPos[3];
};