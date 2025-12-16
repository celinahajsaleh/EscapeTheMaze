#include <GL/freeglut.h>
#include <iostream>
#include "game.h"

using namespace std;

Game* game = nullptr;

void display() {
    if (game) {
        game->render();
    }
}

void idle() {
    if (game) {
        game->update(1.0f / 60.0f);
        glutPostRedisplay();
    }
}

void reshape(int width, int height) {
    if (game) {
        game->handleResize(width, height);
    }
}

void keyboard(unsigned char key, int x, int y) {
    if (game && game->input) {
        game->input->handleKeyPress(key);
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    if (game && game->input) {
        game->input->handleKeyRelease(key);
    }
}

void specialKeyboard(int key, int x, int y) {
    if (game && game->input) {
        game->input->handleSpecialKeyPress(key);
    }
}

void specialKeyboardUp(int key, int x, int y) {
    if (game && game->input) {
        game->input->handleSpecialKeyRelease(key);
    }
}

int main(int argc, char** argv) {
    cout << "Astray - OpenGL Edition" << endl;
    cout << "========================" << endl;
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Astray - OpenGL");
    
    cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
    
    game = new Game();
    game->initialize();
    
    glutDisplayFunc(display);  
    glutIdleFunc(idle);  
    glutReshapeFunc(reshape); 
    glutKeyboardFunc(keyboard);  
    glutKeyboardUpFunc(keyboardUp);  
    glutSpecialFunc(specialKeyboard);  
    glutSpecialUpFunc(specialKeyboardUp);  
    glutSetKeyRepeat(GLUT_KEY_REPEAT_ON);
    
    cout << "\nControls:" << endl;
    cout << "  Arrow keys or h/j/k/l to move" << endl;
    cout << "  Hold 'i' to show instructions" << endl;
    cout << "  ESC to quit" << endl;
    cout << "\nStarting game...\n" << endl;
    
    glutMainLoop();
    
    delete game;
    
    return 0;
}
