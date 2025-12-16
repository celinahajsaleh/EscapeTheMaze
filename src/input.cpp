#include "input.h"
#include <GL/freeglut.h>

Input::Input() {
    keyAxis = glm::vec2(0.0f, 0.0f);
    showInstructions = false;
    upPressed = false;
    downPressed = false;
    leftPressed = false;
    rightPressed = false;
}

void Input::handleKeyPress(unsigned char key) {
    switch (key) {
        case 'i':
        case 'I':
            showInstructions = true;
            break;
        case 'h':
        case 'H':
            leftPressed = true;
            break;
        case 'j':
        case 'J':
            downPressed = true;
            break;
        case 'k':
        case 'K':
            upPressed = true;
            break;
        case 'l':
        case 'L':
            rightPressed = true;
            break;
        case 27: // ESC key
            exit(0);
            break;
    }
    updateKeyAxis();
}

void Input::handleKeyRelease(unsigned char key) {
    switch (key) {
        case 'i':
        case 'I':
            showInstructions = false;
            break;
        case 'h':
        case 'H':
            leftPressed = false;
            break;
        case 'j':
        case 'J':
            downPressed = false;
            break;
        case 'k':
        case 'K':
            upPressed = false;
            break;
        case 'l':
        case 'L':
            rightPressed = false;
            break;
    }
    updateKeyAxis();
}

void Input::handleSpecialKeyPress(int key) {
    switch (key) {
        case GLUT_KEY_LEFT:
            leftPressed = true;
            break;
        case GLUT_KEY_RIGHT:
            rightPressed = true;
            break;
        case GLUT_KEY_UP:
            upPressed = true;
            break;
        case GLUT_KEY_DOWN:
            downPressed = true;
            break;
    }
    updateKeyAxis();
}

void Input::handleSpecialKeyRelease(int key) {
    switch (key) {
        case GLUT_KEY_LEFT:
            leftPressed = false;
            break;
        case GLUT_KEY_RIGHT:
            rightPressed = false;
            break;
        case GLUT_KEY_UP:
            upPressed = false;
            break;
        case GLUT_KEY_DOWN:
            downPressed = false;
            break;
    }
    updateKeyAxis();
}

void Input::updateKeyAxis() {
    keyAxis.x = 0.0f;
    keyAxis.y = 0.0f;
    
    if (leftPressed) keyAxis.x -= 1.0f;
    if (rightPressed) keyAxis.x += 1.0f;
    if (upPressed) keyAxis.y += 1.0f;
    if (downPressed) keyAxis.y -= 1.0f;
}

void Input::reset() {
    keyAxis = glm::vec2(0.0f, 0.0f);
    showInstructions = false;
    upPressed = false;
    downPressed = false;
    leftPressed = false;
    rightPressed = false;
}
