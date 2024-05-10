//#include <iostream>
//#include <vector>
//#include <cmath>
//#include <GL/glut.h>
//#include <windows.h>
//#include <winuser.h>
//
//using namespace std;
//
//float angle = 0.0;
//float lx = 0.0f, lz = -1.0f;
//float X = 0.0f, Z = 5.0f;
//float velocity = 0.1f;
//float deltaAngle = 0.0f;
//float deltaMove = 0;
//float deltaStrafe = 0;
//int xOrigin = -1;
//int mouseX = -1;
//bool deltaChange = false;
//
//vector<pair<float, float>> bullets;
//vector<pair<float, float>> snowmen = {
//    { -10.0f, -10.0f }, { 0.0f, -10.0f }, { 10.0f, -10.0f },
//    { -10.0f, 0.0f }, { 0.0f, 0.0f }, { 10.0f, 0.0f },
//    { -10.0f, 10.0f }, { 0.0f, 10.0f }, { 10.0f, 10.0f }
//};
//const float bulletSpeed = 0.5f;
//const float snowmanSpeed = 0.03f; // Adjust according to your preference
//
//void drawSnowMan()
//{
//    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
//    glTranslatef(0.0f, 0.75f, 0.0f);
//    glutSolidSphere(0.75f, 20, 20);
//    glTranslatef(0.0f, 1.0f, 0.0f);
//    glutSolidSphere(0.25f, 20, 20);
//    glPushMatrix();
//    glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
//    glTranslatef(0.05f, 0.10f, 0.18f);
//    glutSolidSphere(0.05f, 10, 10);
//    glTranslatef(-0.1f, 0.0f, 0.0f);
//    glutSolidSphere(0.05f, 10, 10);
//    glPopMatrix();
//    glColor4f(1.0f, 0.5f, 0.5f, 1.0f);
//    glutSolidCone(0.08f, 0.5f, 10, 2);
//}
//
//void computePosition(float deltaMove, float deltaStrafe)
//{
//    X += ((deltaMove * lx) + (deltaStrafe * lz)) * velocity;
//    Z += ((deltaMove * lz) + (-deltaStrafe * lx)) * velocity;
//}
//
//void computeDirection(float deltaAngleArgument)
//{
//    angle += deltaAngleArgument;
//    lx = sin(angle);
//    lz = -cos(angle);
//    if (deltaChange)
//    {
//        deltaAngle = 0.0f;
//        xOrigin = mouseX;
//        deltaChange = false;
//    }
//}
//
//void mouseButton(int button, int state, int x, int y)
//{
//    if (button == GLUT_RIGHT_BUTTON) {
//        if (state == GLUT_UP) {
//            xOrigin = -1;
//            deltaAngle = 0.0f;
//        }
//        else
//        {
//            mouseX = x;
//            xOrigin = x;
//        }
//    }
//}
//
//void mouseMove(int x, int y)
//{
//    if (xOrigin >= 0) {
//        deltaAngle = (x - xOrigin) * 0.01f;
//        mouseX = x;
//        deltaChange = true;
//    }
//}
//
//void resize(int w, int h)
//{
//    if (h <= 0) h = 1;
//    float ratio = 1.0 * w / h;
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glViewport(0, 0, w, h);
//    gluPerspective(45, ratio, 1, 1000);
//    glMatrixMode(GL_MODELVIEW);
//}
//
//void keyDown(unsigned char key, int x, int y)
//{
//    switch (key)
//    {
//    case 27:
//        exit(0);
//        break;
//
//    case 'a':
//        deltaStrafe += 0.5f;
//        break;
//
//    case 'd':
//        deltaStrafe += -0.5f;
//        break;
//
//    case 'w':
//        deltaMove += 0.5f;
//        break;
//
//    case 's':
//        deltaMove += -0.5f;
//        break;
//
//    case ' ':
//        bullets.push_back(make_pair(X, Z));
//        break;
//    }
//}
//
//void keyUp(unsigned char key, int x, int y)
//{
//    switch (key)
//    {
//    case 'a':
//        deltaStrafe -= 0.5f;
//        break;
//
//    case 'd':
//        deltaStrafe -= -0.5f;
//        break;
//
//    case 'w':
//        deltaMove -= 0.5f;
//        break;
//
//    case 's':
//        deltaMove -= -0.5f;
//        break;
//    }
//}
//
//bool checkCollision(const pair<float, float>& bullet, const pair<float, float>& snowman, float radius)
//{
//    float dx = bullet.first - snowman.first;
//    float dz = bullet.second - snowman.second;
//    float distance = sqrt(dx * dx + dz * dz);
//    return distance < radius;
//}
//
//void updateBullets()
//{
//    vector<pair<float, float>> newBullets;
//    for (const auto& bullet : bullets)
//    {
//        float bulletX = bullet.first + bulletSpeed * lx;
//        float bulletZ = bullet.second + bulletSpeed * lz;
//        bool hit = false;
//        for (auto it = snowmen.begin(); it != snowmen.end(); )
//        {
//            if (checkCollision(bullet, *it, 1.0))
//            {
//                it = snowmen.erase(it);
//                hit = true;
//            }
//            else
//            {
//                ++it;
//            }
//        }
//        if (!hit)
//        {
//            newBullets.push_back(make_pair(bulletX, bulletZ));
//        }
//    }
//    bullets = newBullets;
//}
//
//void updateSnowmen()
//{
//    for (auto& snowman : snowmen)
//    {
//        // Calculate direction vector from snowman to player
//        float dx = X - snowman.first;
//        float dz = Z - snowman.second;
//        float distance = sqrt(dx * dx + dz * dz);
//
//        // Normalize direction vector
//        if (distance != 0) {
//            dx /= distance;
//            dz /= distance;
//        }
//
//        // Update snowman position towards player
//        snowman.first += dx * snowmanSpeed;
//        snowman.second += dz * snowmanSpeed;
//    }
//}
//
//void renderScene(void)
//{
//    if (deltaMove || deltaStrafe) computePosition(deltaMove, deltaStrafe);
//    if (deltaAngle || deltaStrafe) computeDirection(deltaAngle);
//
//    updateBullets();
//    updateSnowmen(); // Call the function to update snowmen positions
//
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glLoadIdentity();
//
//    gluLookAt(X, 1.0f, Z,
//        X + lx, 1.0f, Z + lz,
//        0.0f, 1.0f, 0.0f);
//
//    glColor4f(0.9f, 0.9f, 0.9f, 1.0f);
//    glBegin(GL_QUADS);
//    glVertex3f(-100.0f, 0.0f, -100.0f);
//    glVertex3f(-100.0f, 0.0f, 100.0f);
//    glVertex3f(100.0f, 0.0f, 100.0f);
//    glVertex3f(100.0f, 0.0f, -100.0f);
//    glEnd();
//
//    for (const auto& snowman : snowmen)
//    {
//        glPushMatrix();
//        glTranslatef(snowman.first, 0, snowman.second);
//        drawSnowMan();
//        glPopMatrix();
//    }
//
//    glPointSize(5);
//    glBegin(GL_POINTS);
//    glColor3f(1.0, 0.0, 0.0);
//    for (const auto& bullet : bullets) {
//        glVertex3f(bullet.first, 0.5f, bullet.second);
//    }
//    glEnd();
//
//    glutSwapBuffers();
//}
//
//int main(int argc, char** argv)
//{
//    const long screenWidth = ::GetSystemMetrics(SM_CXSCREEN);
//    const long screenHeight = ::GetSystemMetrics(SM_CYSCREEN);
//    long windowWidth = 0.9 * screenWidth;
//    long windowHeight = 0.9 * screenHeight;
//    long windowPositionX = 0.05 * screenWidth;
//    long windowPositionY = 0.05 * screenHeight;
//
//    glutInit(&argc, argv);
//    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
//    glutInitWindowPosition(windowPositionX, windowPositionY);
//    glutInitWindowSize(windowWidth, windowHeight);
//    glutCreateWindow("Untitled FPS Game");
//
//    glutDisplayFunc(renderScene);
//    glutReshapeFunc(resize);
//    glutIdleFunc(renderScene);
//    glutKeyboardFunc(keyDown);
//    glutIgnoreKeyRepeat(1);
//    glutKeyboardUpFunc(keyUp);
//    glutMouseFunc(mouseButton);
//    glutMotionFunc(mouseMove);
//
//    glEnable(GL_DEPTH_TEST);
//
//    glutMainLoop();
//
//    return 0;
//}
