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

#include <vector>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#include "Terrain.h"
#include "ParticleSystem.h"
#include "Camera.h"


/*****************************************
*    FUNCTION DECLARATIONS
****************************************/
void drawAxes();
void drawHud();
void togglePausedScene();
void passive(int x, int y);
void timer(int value);

/*****************************************
 *    GLOBAL VARIABLES
 ****************************************/
Terrain terrain;
ParticleSystem volcanoParticles(&terrain);
Camera camera;

bool fullscreen = false;
bool paused = false;
float lightPos[4] = {0,60,0, 1};

bool mouseCurrentInitiated = false;
int currX = 0;
int currY = 0;
int windowWidth = 800;
int windowHeight = 600;

int hudWidth = 0;
int hudHeight = 0;
GLubyte *hudImage;

/*****************************************
* draws scene
****************************************/
void display(void) {
    
    //clear bits and model view matrix
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //transform according to camera
    glRotatef(camera.rotation[0], 1, 0, 0);
    glRotatef(camera.rotation[1], 0, 1, 0);
    glTranslatef(-camera.position[0], -terrain.getHeight(camera.position[0], camera.position[2])-2, -camera.position[2]);

    //draw the scene
    drawAxes();
    terrain.drawTerrain();
    volcanoParticles.drawParticles();
    if (paused)
        drawHud();
    
    glutSwapBuffers();
}

//TODO: rewrite this function so it's not as ugly/messy
GLubyte* LoadPPM(char* file, int* width, int* height) {

	//open the file in read mode
	FILE *fd = fopen(file, "r");
    if (fd == NULL) {
        printf("Error. File \"%s\" could not be loaded.",file);
        return NULL;
    }
    
	//scan everything up to newline
    char b[100];
	fscanf(fd,"%[^\n] ", b);

	//check if the first two characters are not P3, if not, it's not an ASCII PPM file
	if (b[0]!='P'|| b[1] != '3') {
		printf("%s is not a PPM file!\n",file);
		return NULL;
	}

	//read past the file comments: scan for lines that begin
	//  with #, and keep going until you find no more
    char c;
    fscanf(fd, "%c",&c);
	while(c == '#')	{
		fscanf(fd, "%[^\n] ", b);
		fscanf(fd, "%c",&c);
	}

	//rewind the read pointer one character, or we'll lose the size
	ungetc(c,fd);

	//read the rows, columns and max colour values
    int k, n, m;
	fscanf(fd, "%d %d %d", &n, &m, &k);

	//number of pixels is rows * columns
    int size = n*m;

	//allocate memory to store 3 GLuints for every pixel
	GLubyte* img = (GLubyte *)malloc(3*sizeof(GLuint)*size);

	//scale the colour in case maxCol is not 255
    float s;
    s = 255.0/k;

	//start reading pixel colour data
    int red, green, blue;
	for(int i = 0; i < size; i++) {
		fscanf(fd,"%d %d %d",&red, &green, &blue );
		img[3*size-3*i-3]=red*s;
		img[3*size-3*i-2]=green*s;
		img[3*size-3*i-1]=blue*s;
	}

	*width = n;
	*height = m;

	return img;
}

/********************************************
* draws the menu for when game paused
*******************************************/
void drawHud() {
    glDisable(GL_DEPTH_TEST);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, windowWidth, 0, windowHeight);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRasterPos2i(windowWidth/2+hudWidth/2, windowHeight/2-hudHeight/2);
    glPixelZoom(-1,1);
    glDrawPixels(hudWidth, hudHeight, GL_RGB, GL_UNSIGNED_BYTE, hudImage);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float) windowWidth / (float) windowHeight, 1,400);
    
    glEnable(GL_DEPTH_TEST);
}

/********************************************
* pauses or unpauses scene. If pausing, draws
* the menu (hud) and disables interactivity
* except with the menu or unpausing or quitting
*******************************************/
void togglePausedScene() {
    
    paused = !paused;

    if (!paused) {
        glutPassiveMotionFunc(passive);
        glutSetCursor(GLUT_CURSOR_NONE);
    }
    
    else {
        glutPassiveMotionFunc(NULL);
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    }
}

/********************************************
* handles key presses for program functions
*******************************************/
void keyboard(unsigned char key, int x, int y) {
    
    //keys that are handled whether paused or not
    switch (key) {
        
        //quit
        case 'q':
            exit(0);
            break;
        
        //pause
        case 'p':
        case 'P':
            togglePausedScene();
            break;

        //toggle fullscreen
        case 'f':
        case 'F':
            if (!fullscreen)
                glutFullScreen();
            //TODO: when paused it won't exit fullscreen (it will enter it though)
            else {
                glutPositionWindow(10, 10);
                glutReshapeWindow(800, 600);
            }
            break;
    }
    
    //keys that only work when not paused
    if (!paused) {
        switch (key) {
            
            case 'r':
                terrain.generateTerrain();
                break;
            case '1':
                glShadeModel(GL_FLAT);
                break;
            case '2':
                glShadeModel(GL_SMOOTH);
                break;
            case '3':
                volcanoParticles.shape = 0;
                break;
            case '4':
                volcanoParticles.shape = 1;
                break;
            
            //move player
            case 'w':
            case 'W':
                camera.strafe(Camera::FORWARD);
                break;
            case 's':
            case 'S':
                camera.strafe(Camera::BACK);
                break;
            case 'a':
            case 'A':
                camera.strafe(Camera::LEFT);
                break;
            case 'd':
            case 'D':
                camera.strafe(Camera::RIGHT);
                break;
        }
    }
    glutPostRedisplay();
}

/********************************************
* moves camera (First Person)
*******************************************/
void passive(int x, int y) {

    //on first call set currX, currY
    if (!mouseCurrentInitiated) {
        currX = x;
        currY = y;
        mouseCurrentInitiated = true;
    }
    
    //if cursor approaching edge, set it to middle of window
    if (x < 1 || x >= windowWidth-1) {
        glutWarpPointer(windowWidth/2.0, y);
        currX = windowWidth/2.0;
    }
    if (y < 1 || y >= windowHeight-1) {
        glutWarpPointer(x, windowHeight/2.0);
        currY = windowHeight/2.0;
    }
    
    //move camera according to mouse movement
    camera.mouseMoved(x-currX, y-currY);
    currX = x;
    currY = y;
    
    glutPostRedisplay();
}
void mouse(int button, int state, int x, int y) {
    if (state == GLUT_LEFT_BUTTON) {
        printf("mouse clicked at %d %d",x,y);
    }
}

/********************************************
* moves volcano particles
*******************************************/
void timer(int value) {
    
    if (!paused)
        volcanoParticles.moveParticles();
    
    //set timer function
    glutTimerFunc(32, timer, 0);
    glutPostRedisplay();
}

void drawAxes() {
    glDisable(GL_LIGHTING);
    
    glBegin(GL_LINES);
    glColor3f(1, 0, 0);
    glVertex3f(0,0,0);
    glVertex3f(500, 0, 0);
    
    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 500, 0);
    
    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 500);
    glEnd();
    
    glEnable(GL_LIGHTING);
}

/********************************************
* sets viewport according to window size
*******************************************/
void reshape(int w, int h) {
    
    //don't let window become less than 300 x 300
    int minWindowSize = 300;
    if (w < minWindowSize || h < minWindowSize) {
        glutReshapeWindow((w < minWindowSize) ? minWindowSize : w, (h < minWindowSize) ? minWindowSize : h);
        windowWidth = (w < minWindowSize) ? 300 : w;
        windowHeight = (h < minWindowSize) ? 300 : h;
    }
    
    //change projection matrix, set width & height globals
    else {
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
     
        glViewport(0, 0, (GLsizei) w, (GLsizei) h);
        gluPerspective(45, (GLfloat) w / (GLfloat) h, 1,400);
        
        windowWidth = w;
        windowHeight = h;
    }
    
    glutPostRedisplay();
}

/*******************************************
*initializes global variables and settings
******************************************/
void init() {
    
    //enable flat shading (for artistic reasons)
    glShadeModel(GL_FLAT);
    
    glClearColor(0.25, 0.53, 0.77, 1);
    glEnable(GL_DEPTH_TEST);
    
    glFrontFace(GL_CCW);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    
    //set projection matrix, using perspective w/ correct aspect ratio
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45,(GLfloat) glutGet(GLUT_WINDOW_WIDTH) / (GLfloat) glutGet(GLUT_WINDOW_HEIGHT), 1, 400);
    
    //initialize globals
    terrain = Terrain();
    volcanoParticles = ParticleSystem(&terrain);

    //setup interface image
    char currentDir[1024];
    char* fileName = (char*) "/interface.ppm";
    if (getcwd(currentDir, sizeof(currentDir)) != NULL) {
        char* filePath;
        filePath = (char*) malloc(strlen(currentDir)+34);
        strcpy(filePath, currentDir);
        strcat(filePath, fileName);
        printf("filepath: \"%s\"\n",filePath);
        hudImage = LoadPPM(filePath, &hudWidth, &hudHeight);
        free(filePath);
    }
    else {
        printf("Error. Current directory path too long");
    }
    
    //initialize camera
    camera = Camera();
    
    //hide cursor
    glutSetCursor(GLUT_CURSOR_NONE);
}

/*****************************************
* program start point
****************************************/
int main(int argc, char** argv) {
    
    //initializeing GLUT
    glutInit(&argc, argv);
    
    //making our window
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(10, 10);
    glutCreateWindow("Volcano");
    
    //initializing variables
    init();
    
    //registering callbacks
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutReshapeFunc(reshape);
    glutPassiveMotionFunc(passive);
    glutMouseFunc(mouse);
    glutTimerFunc(32, timer, 0);
    
    //start event loop
    glutMainLoop();
    
    return(0);
}