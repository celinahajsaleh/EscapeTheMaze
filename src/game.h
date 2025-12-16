#ifndef GAME_H
#define GAME_H

#include "maze.h"
#include "physics.h"
#include "camera.h"
#include "input.h"
#include "audio.h"
#include <glm/glm.hpp>

enum GameState {
    INITIALIZE,
    FADE_IN,
    PLAY,
    FADE_OUT
};

class Game {
public:
    Maze* maze;
    Physics* physics;
    Camera* camera;
    Input* input;
    Audio* audio;
    
    GameState state;
    int mazeDimension;
    int currentLevel;
    
    glm::vec3 ballRenderPosition;
    glm::mat4 ballRotationMatrix;
    float ballTotalRotation;
    glm::vec3 ballRotationAxis;
    glm::vec3 lastBallPosition;
    
    float lightIntensity;
    
    Game();
    ~Game();
    
    void initialize();
    void update(float deltaTime);
    void render();
    void handleResize(int width, int height);
    
private:
    void initializeLevel();
    void updatePhysics(float deltaTime);
    void updateRenderWorld();
    void checkVictory();
    void nextLevel();
    
    void fadeIn();
    void fadeOut();
    
    const float FADE_STEP = 0.1f;
};

#endif
