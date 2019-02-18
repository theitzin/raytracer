#include <GL/glut.h>
#include "handler.hpp"
#include <iostream>

Handler handler;
int mouseLastX = 0;
int mouseLastY = 0;
bool mouseOn = false;
bool oldMouseData = true;

void draw(void) {   
    handler.drawmode->draw();
}

void idle() {
    if (handler.drawmode->finished()) {
        glutIdleFunc(NULL);
        return;
    }
    
    for (int i = 0; i < handler.getBatchSize(); i++)
        if (!handler.drawmode->finished())
            handler.drawmode->drawNext();

}

void handleKeypress(unsigned char key, int x, int y) {

    float dx = 0.0f;
    float dz = 0.0f;

    switch(key) {
    case 27:
        exit(0); break;
    case 119: 
        dz = 1.0f; break;
    case 97: 
        dx = -1.0f; break;
    case 115: 
        dz = -1.0f; break;
    case 100: 
        dx = 1.0f; break;
    case 32:
        mouseOn = !mouseOn; 
        oldMouseData = true; break;
    }

    if (!handler.camera || !handler.drawmode)
        return;

    if (dx != 0.0f || dz != 0.0f) {

        handler.drawmode->setFinishedState(false);
        glutIdleFunc(idle);
        handler.camera->updateOrigin(Vec3<float>(dx, 0.0f, dz));
        handler.drawmode->updateWindowContent();
    }
}

void passiveMouse(int x, int y) { 

    if (!handler.camera || !handler.drawmode || !mouseOn)
        return;

    float dx = 0.0f;
    float dy = 0.0f;

    if (oldMouseData) {
        oldMouseData = false;
    }
    else {
        dx = (x - mouseLastX) / 100.0f;
        dy = (y - mouseLastY) / 100.0f;  
    }

    if (dx != 0.0f || dy != 0.0f) {

        handler.drawmode->setFinishedState(false);
        glutIdleFunc(idle);
        handler.camera->updateRotation(dy, dx);
        handler.drawmode->updateWindowContent();
    }

    mouseLastX = x;
    mouseLastY = y;
}

void handleResize(int w,int h) {
    handler.resize(w, h);

    handler.drawmode->setFinishedState(false);
    glutIdleFunc(idle);
    handler.drawmode->updateWindowContent();
}

void update(int value) {

    glutPostRedisplay();
    glutTimerFunc(50, update, 0);
}

//Main program

int main(int argc, char **argv) {

    glutInit(&argc, argv);

    /*Setting up  The Display
    /    -RGB color model + Alpha Channel = GLUT_RGBA
    */
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);

    //Configure Window Postion
    glutInitWindowPosition(50, 25);

    //Configure Window Size
    glutInitWindowSize(handler.getWindowWidth(), handler.getWindowHeight());

    //Create Window
    glutCreateWindow("raytracer");

    //Call to the drawing function
    glutDisplayFunc(draw);
    glutKeyboardFunc(handleKeypress);
    glutPassiveMotionFunc(passiveMouse);
    glutReshapeFunc(handleResize);
    glutTimerFunc(50, update, 0);
    glutIdleFunc(idle);
    handler.glInit();

    // Loop require by OpenGL
    glutMainLoop();

    return 0;
}