#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <windows.h>
#include <winuser.h>
using namespace std;

float angle = 0.0;              //Angle of rotation for camera direction
float lx = 0.0f, lz = -1.0f;    //Vector representing camera direction
float X = 0.0f, Z = 5.0f;       //XZ position of the camera
float velocity = 0.1f;
//Key states - 0 when not pressed
float deltaAngle = 0.0f;
float deltaMove = 0;

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

//Draw snowman
void drawSnowMan()
{
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

    //Body
    glTranslatef(0.0f, 0.75f, 0.0f);
    glutSolidSphere(0.75f, 20, 20);

    //Head
    glTranslatef(0.0f, 1.0f, 0.0f);
    glutSolidSphere(0.25f, 20, 20);

    //Eyes
    glPushMatrix();
    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);

    glTranslatef(0.05f, 0.10f, 0.18f);
    glutSolidSphere(0.05f, 10, 10);

    glTranslatef(-0.1f, 0.0f, 0.0f);
    glutSolidSphere(0.05f, 10, 10);

    glPopMatrix();

    //Nose
    glColor4f(1.0f, 0.5f, 0.5f, 1.0f);
    glutSolidCone(0.08f, 0.5f, 10, 2);
}

//Function to calculate position depending on key press
void computePosition(float deltaMove)
{
    X += deltaMove * lx * velocity;
    Z += deltaMove * lz * velocity;
}

//Function to calculate direction depending on key press
void computeDirection(float deltaAngle)
{
    angle += deltaAngle;
    lx = sin(angle);
    lz = -cos(angle);
}

// Display callback function
void renderScene(void)
{
    if (deltaMove) computePosition(deltaMove);
    if (deltaAngle) computeDirection(deltaAngle);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);             //Reset colour and depth buffers
    glLoadIdentity();                                               //Reset transformations

    //Set camera
    gluLookAt(X, 1.0f, Z,                                    
              X+lx, 1.0f, Z+lz,
              0.0f, 1.0f, 0.0f);

    //Draw ground
    glColor4f(0.9f, 0.9f, 0.9f, 1.0f);
    glBegin(GL_QUADS);

    glVertex3f(-100.0f, 0.0f, -100.0f);
    glVertex3f(-100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, -100.0f);

    glEnd();

    //36 snowmen
    for (int i = -3; i < 3; i++)
    {
        for (int j = -3; j < 3; j++)
        {
            glPushMatrix();
            glTranslatef(i * 10.0, 0, j * 10.0);
            drawSnowMan();
            glPopMatrix();
        }
    }

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
void keyDown(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        exit(0);
        break;

    case 'a':
        deltaAngle = -0.01f;
        break;

    case 'd':
        deltaAngle += 0.01f;
        break;

    case 'w':
        deltaMove = 0.5f;
        break;

    case 's':
        deltaMove = -0.5f;
        break;
    }
}

void keyUp(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'a':

    case 'd':
        deltaAngle = 0.0f;
        break;

    case 'w':

    case 's':
        deltaMove = 0.0f;
        break;
    }
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

    glutKeyboardFunc(keyDown);
    glutIgnoreKeyRepeat(1);
    glutKeyboardUpFunc(keyUp);

    glEnable(GL_DEPTH_TEST);                    //OpenGL init

    glutMainLoop();

    return 0;
}