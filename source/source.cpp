#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <windows.h>
#include <winuser.h>
using namespace std;

float angle = 0.0;              //Angle of rotation for camera direction
float lx = 0.0f, lz = -1.0f;    //Vector representing camera direction
float x = 0.0f, z = 5.0f;       //XZ position of the camera

// Initialise the window
void Initialise(int *argc, char **argv)
{
    const long screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
    const long screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
    long windowWidth = 0.9 * screenWidth;
    long windowHeight = 0.9 * screenHeight;
    long windowPositionX = 0.05 * screenWidth;
    long windowPositionY = 0.05 * screenHeight;

    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowPosition(windowPositionX, windowPositionY);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Untitled FPS Game");
}

// Display callback function
void renderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);             //Reset colour and depth buffers
    glLoadIdentity();                                               //Reset transformations

    //Set camera
    gluLookAt(0.0f, 0.0f, 10.0f,                                    
              0.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f);

    glRotatef(angle, 0.0f, 0.0f, 1.0f);                             //Rotate for the tutorial

    glutSwapBuffers();
}

//Resize function
void resize(int w, int h)
{
    if (h <= 0) h = 1;                          //Check for division by 0

    float ratio = 1.0 * w / h;                  //Compute window ratio

    glMatrixMode(GL_PROJECTION);                //Use the projection matrix - defines the viewing volume
    glLoadIdentity();                           //Load identity matrix for initialisation
    glViewport(0, 0, w, h);                     //Set viewport to be the whole window
    gluPerspective(45, ratio, 1, 1000);         //Set perspective parameters
    glMatrixMode(GL_MODELVIEW);                 //Use modelview matrix
}

//Normal key input function
void normalKeys(unsigned char key, int x, int y)
{
    if (key == 27) exit(0);
}

//Special key input function
void specialKeys(int key, int x, int y)
{
    
}

//Main
int main(int argc, char **argv)
{
    Initialise(&argc, argv);                    //Call the user-made initialisation function

    glutDisplayFunc(renderScene);               //Set the display callback function
    glutReshapeFunc(resize);                    //Set the window reshape callback function
    glutIdleFunc(renderScene);                  //Set the idle callback function
    glutKeyboardFunc(normalKeys);               //Set the normal key press callback function
    glutSpecialFunc(specialKeys);               //Set the special key press callback function

    glutMainLoop();

    return 0;
}