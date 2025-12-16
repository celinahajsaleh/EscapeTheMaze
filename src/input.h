#ifndef INPUT_H
#define INPUT_H

#include <glm/glm.hpp>

class Input {
public:
    glm::vec2 keyAxis;
    bool showInstructions;
    
    Input();
    
    void handleKeyPress(unsigned char key);
    void handleKeyRelease(unsigned char key);
    void handleSpecialKeyPress(int key);
    void handleSpecialKeyRelease(int key);
    
    void reset();
    
private:
    bool upPressed;
    bool downPressed;
    bool leftPressed;
    bool rightPressed;
    
    void updateKeyAxis();
};

#endif
