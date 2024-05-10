#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include <windows.h>
#include <winuser.h>
#include <vector>
#include <cmath>
using namespace std;

float angle = 0.0;                          //Angle of rotation for camera direction
float lx = 0.0f, lz = -1.0f;                //Vector representing camera direction
float X = 0.0f, Y = 1.0f, Z = 5.0f;         //XYZ position of the camera
float playerVelocity = 0.1f;
float bulletVelocity = 0.5f;
float enemyVelocity = 0.03f;
float deltaAngle = 0.0f;
float deltaMove = 0;
float deltaStrafe = 0;
double deltaY = 0.003;
bool bobUp = true;
int xOrigin = -1;
int mouseX = -1;                            //To update xOrigin while moving camera
bool deltaChange = false;

vector<pair<float, float>> bullets;
vector<pair<float, float>> enemyVector;

void drawEnemy(float dx, float dz)
{
    // Calculate the angle between the snowman and the player
    float snowmanAngle = 0.0f;
    if (dx != 0.0f || dz != 0.0f) {
        snowmanAngle = atan2(-dz, -dx) * 180.0f / 3.14159265358979323846;
    }

    //glPushMatrix();
    glRotatef(snowmanAngle, 0.0f, 1.0f, 0.0f);

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


void updatePositions()
{
    for (auto& enemy : enemyVector)
    {
        float dx = X - enemy.first;
        float dz = Z - enemy.second;
        float distance = sqrt((dx * dx) + (dz * dz));

        if (distance != 0)
        {
            dx /= distance;
            dz /= distance;
        }

        enemy.first += dx * enemyVelocity;
        enemy.second += dz * enemyVelocity;
    }
}

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

//Function to calculate position depending on key press
void computePosition(float deltaMove, float deltaStrafe)
{
    float deltaX = ((deltaMove * lx) + (deltaStrafe * lz)) * playerVelocity;
    float deltaZ = ((deltaMove * lz) + (-deltaStrafe * lx)) * playerVelocity;
    bool updateX = false;
    bool updateZ = false;

    

    if ((X > -199) && ((X + deltaX) < X))
    {
        X += deltaX;
        updateX = true;
    }
    if ((X < 199) && ((X + deltaX) > X))
    {
        X += deltaX;
        updateX = true;
    }
    if ((Z > -199) && ((Z + deltaZ) < Z))
    {
        Z += deltaZ;
        updateZ = true;
    }
    if ((Z < 199) && ((Z + deltaZ) > Z))
    {
        Z += deltaZ;
        updateZ = true;
    }

    if (updateX || updateZ)
    {
        if (bobUp && ((Y + deltaY) <= 1.2f)) Y += deltaY;
        if (bobUp && ((Y + deltaY) > 1.2f))
        {
            deltaY = 0.003;
            bobUp = false;
        }
        if (!bobUp && ((Y - deltaY) >= 1.0f)) Y -= deltaY;
        if (!bobUp && ((Y - deltaY) < 1.0f))
        {
            deltaY = 0.003;
            bobUp = true;
        }
    }
}

//Function to calculate direction depending on key press
void computeDirection(float deltaAngleArgument)
{
    angle += deltaAngleArgument;
    lx = sin(angle);
    lz = -cos(angle);

    if (deltaChange)
    {
        deltaAngle = 0.0f;
        xOrigin = mouseX;
        deltaChange = false;
    }
}

bool bulletCollision(const pair<float, float>& bullet, const pair<float, float>& enemy, float radius)
{
    float dx = bullet.first - enemy.first;
    float dz = bullet.second - enemy.second;
    float distance = sqrt((dx * dx) + (dz * dz));
    return distance < radius;
}

bool enemySelfCollision(const pair<float, float>& enemy1, const pair<float, float>& enemy2, float radius)
{
    float dx = enemy1.first - enemy2.first;
    float dz = enemy1.second - enemy2.second;
    float distance = sqrt((dx * dx) + (dz * dz));
    return distance < radius;
}

bool playerCollision(const pair<float, float>& player, const pair<float, float>& enemy, float radius)
{
    float dx = player.first - enemy.first;
    float dz = player.second - enemy.second;
    float distance = sqrt((dx * dx) + (dz * dz));
    return distance < radius;
}

void updateBullets()
{
    vector<pair<float, float>> newBullets;
    for (auto& bullet : bullets)
    {
        float bulletX = bullet.first + bulletVelocity * lx;
        float bulletZ = bullet.second + bulletVelocity * lz;
        bool hit = false;

        for (auto it = enemyVector.begin(); it != enemyVector.end(); )
        {
            if (bulletCollision(bullet, *it, 1.0))
            {
                it = enemyVector.erase(it);
                hit = true;
            }
            else
            {
                ++it;
            }
        }
        if (!hit)
        {
            newBullets.push_back(make_pair(bulletX, bulletZ));
        }
    }

    bullets = newBullets;
}

//Function to register mouse button press
void mouseButton(int button, int state, int x, int y)
{
    if (button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_UP)
        {
            xOrigin = -1;
            deltaAngle = 0.0f;
        }
        else
        {
            mouseX = x;
            xOrigin = x;
        }
    }

    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            bullets.push_back(make_pair(X, Z));
        }
    }
}

//Function to register mouse movement when a button is pressed
void mouseMove(int x, int y)
{
    if (xOrigin >= 0) {
        deltaAngle = (x - xOrigin) * 0.01f;
        mouseX = x;
        deltaChange = true;
    }
}

// Display callback function
void renderScene(void)
{
    if (deltaMove || deltaStrafe) computePosition(deltaMove, deltaStrafe);
    if (deltaAngle || deltaStrafe) computeDirection(deltaAngle);

    updateBullets();
    updatePositions();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);             //Reset colour and depth buffers
    glLoadIdentity();                                               //Reset transformations

    //Set camera
    gluLookAt(X, Y, Z,                                    
              X+lx, Y, Z+lz,
              0.0f, 1.0f, 0.0f);

    //Draw ground
    glColor4f(0.01f, 0.9f, 0.1f, 1.0f);
    glBegin(GL_QUADS);

    glVertex3f(-200.0f, 0.0f, -200.0f);
    glVertex3f(-200.0f, 0.0f, 200.0f);
    glVertex3f(200.0f, 0.0f, 200.0f);
    glVertex3f(200.0f, 0.0f, -200.0f);

    glEnd();

    for (auto enemy : enemyVector)
    {
        float dx = X - enemy.first;
        float dz = Z - enemy.second;

        glPushMatrix();
        glTranslatef(enemy.first, 0, enemy.second);
        drawEnemy(dx, dz);
        glPopMatrix();
    }

    glPointSize(5);
    glBegin(GL_POINTS);

    glColor3f(1.0, 0.0, 0.0);
    for (const auto& bullet : bullets)
    {
        glVertex3f(bullet.first, 0.5f, bullet.second);
    }

    glEnd();

    // Draw the crosshair
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);

    glVertex3f(X + lx - 0.02f, Y, Z + lz - 0.001);
    glVertex3f(X + lx + 0.02f, Y, Z + lz - 0.001);
    glVertex3f(X + lx, Y - 0.02f, Z + lz - 0.001);
    glVertex3f(X + lx, Y + 0.02f, Z + lz - 0.001);

    glEnd();

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
        if (deltaStrafe != 0.5f) deltaStrafe += 0.5f;
        break;

    case 'd':
        if (deltaStrafe != -0.5f) deltaStrafe += -0.5f;
        break;

    case 'w':
        if (deltaMove != 0.5f) deltaMove += 0.5f;
        break;

    case 's':
        if (deltaMove != -0.5f) deltaMove += -0.5f;
        break;
    }
}

void keyUp(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'a':
        if (deltaStrafe != -0.5f) deltaStrafe -= 0.5f;
        break;

    case 'd':
        if (deltaStrafe != 0.5f) deltaStrafe -= -0.5f;
        break;

    case 'w':
        if (deltaMove != -0.5f) deltaMove -= 0.5f;
        break;

    case 's':
        if (deltaMove != 0.5f) deltaMove -= -0.5f;
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

    //36 snowmen
    for (int i = -3; i < 3; i++)
    {
        for (int j = -3; j < 3; j++)
        {
            enemyVector.push_back({i * 10.0, j * 10.0});
        }
    }

    glutDisplayFunc(renderScene);               //Set the display callback function
    glutReshapeFunc(resize);                    //Set the window reshape callback function
    glutIdleFunc(renderScene);                  //Set the idle callback function

    glutKeyboardFunc(keyDown);
    glutIgnoreKeyRepeat(1);
    glutKeyboardUpFunc(keyUp);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMove);

    glEnable(GL_DEPTH_TEST);                    //OpenGL init

    glutMainLoop();

    return 0;
}