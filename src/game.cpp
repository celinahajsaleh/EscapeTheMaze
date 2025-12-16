#include "game.h"
#include <GL/freeglut.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <sstream>

using namespace std;

Game::Game() {
    state = INITIALIZE;
    mazeDimension = 11;
    currentLevel = 1;
    ballTotalRotation = 0.0f;
    ballRotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
    ballRotationMatrix = glm::mat4(1.0f);
    lightIntensity = 0.0f;
}

Game::~Game() {
    delete maze;
    delete physics;
    delete camera;
    delete input;
    delete audio;
}

void Game::initialize() {
    camera = new Camera(60.0f, 800.0f / 600.0f, 1.0f, 1000.0f);
    input = new Input();
    
    audio = new Audio();
    audio->initialize();
    audio->generateSounds();
    audio->setSoundVolume(100);
    audio->startRolling();
    audio->updateRollingVolume(0.05f);
    
    initializeLevel();
    
    state = FADE_IN;
}

void Game::initializeLevel() {
    if (maze) delete maze;
    if (physics) delete physics;
    
    maze = new Maze(mazeDimension);
    maze->generate();
    
    physics = new Physics();
    physics->createBall(1.0f, 1.0f, 0.25f);
    
    for (int y = 0; y < mazeDimension; y++) {
        for (int x = 0; x < mazeDimension; x++) {
            if (maze->isWall(x, y)) {
                physics->createWall(x, y, 1.0f, 1.0f);
            }
        }
    }
    
    camera->position = glm::vec3(1.0f, 1.0f, 5.0f);
    camera->setTarget(glm::vec3(1.0f, 1.0f, 0.0f), 5.0f);
    
    ballRenderPosition = glm::vec3(1.0f, 1.0f, 0.25f);
    ballTotalRotation = 0.0f;
    ballRotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
    ballRotationMatrix = glm::mat4(1.0f);
    
    cout << "Level " << currentLevel << " initialized (Dimension: " << mazeDimension << ")" << endl;
}

void Game::update(float deltaTime) { 
    switch (state) { 
        case INITIALIZE: 
            initialize(); 
            break;
            
        case FADE_IN:
            fadeIn();
            break;
            
        case PLAY:
            updatePhysics(deltaTime);
            updateRenderWorld();
            checkVictory();
            break;
            
        case FADE_OUT:
            fadeOut();
            break;
    }
    
    camera->update();
}

void Game::updatePhysics(float deltaTime) {
    // Apply force from input
    if (glm::length(input->keyAxis) > 0.0f) {
        physics->applyForce(input->keyAxis.x, input->keyAxis.y);
    }
    
    // Get velocity before update
    glm::vec2 oldVelocity = physics->getBallVelocity();
    float oldSpeed = glm::length(oldVelocity);
    
    // Update physics simulation
    physics->update(deltaTime);
    
    // Get velocity after update
    glm::vec2 newVelocity = physics->getBallVelocity();
    float newSpeed = glm::length(newVelocity);
    
    if (newSpeed > 0.15f) {
        audio->startMovement();
        audio->updateMovementVolume(min(1.0f, newSpeed / 3.0f));
    } else {
        audio->stopMovement();
    }
    
    float velocityDrop = oldSpeed - newSpeed;
    if (velocityDrop > 1.5f && oldSpeed > 0.5f) {
        audio->playSound("collision", 0);
    }
}

void Game::updateRenderWorld() {
    glm::vec2 physicsPos = physics->getBallPosition();
    glm::vec3 newBallPosition = glm::vec3(physicsPos.x, physicsPos.y, 0.25f);
    
    float stepX = newBallPosition.x - ballRenderPosition.x;
    float stepY = newBallPosition.y - ballRenderPosition.y;
    
    if (abs(stepX) > 0.0001f || abs(stepY) > 0.0001f) {
        float ballRadius = 0.25f;
        float rotationY = stepX / ballRadius;
        float rotationX = -stepY / ballRadius;
        glm::vec3 axis = glm::normalize(glm::vec3(rotationX, rotationY, 0.0f));
        float angle = glm::length(glm::vec2(rotationX, rotationY));
        ballTotalRotation += angle;
        ballRotationAxis = axis;
        
        // Update rotation matrix
        glm::mat4 deltaRotation = glm::rotate(glm::mat4(1.0f), angle, axis);
        ballRotationMatrix = deltaRotation * ballRotationMatrix;
    }
    
    ballRenderPosition = newBallPosition;
    
    // Update camera target
    camera->setTarget(ballRenderPosition, 5.0f);
}

void Game::checkVictory() {
    glm::vec2 ballPos = physics->getBallPosition();
    int ballX = static_cast<int>(ballPos.x + 0.5f);
    int ballY = static_cast<int>(ballPos.y + 0.5f);
    
    // Check if ball reached exit
    if (ballX == mazeDimension - 1 && ballY == mazeDimension - 2) {
        audio->playVictory();
        state = FADE_OUT;
    }
}

void Game::nextLevel() {
    currentLevel++;
    mazeDimension += 2;
    initializeLevel();
    state = FADE_IN;
}

void Game::fadeIn() {
    lightIntensity = lightIntensity + FADE_STEP;
    
    if (lightIntensity >= 1.0f) {
        lightIntensity = 1.0f;
        state = PLAY;
        cout << "Fade in complete, starting gameplay" << endl;
    }
}

void Game::fadeOut() {
    lightIntensity = lightIntensity - FADE_STEP;
    
    if (lightIntensity <= 0.0f) {
        lightIntensity = 0.0f;
        nextLevel();
    }
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    
    // Set viewport
    glViewport(0, 0, 800, 600);
    
    // Setup 3D perspective
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0, 800.0/600.0, 0.1, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Top down camera view with zoom  looking straight down at the ball
    glm::vec3 camPos = ballRenderPosition + glm::vec3(0.0f, 0.0f, 3.5f);
    
    gluLookAt(camPos.x, camPos.y, camPos.z,           // Eye position (above)
              ballRenderPosition.x, ballRenderPosition.y, ballRenderPosition.z,  // Look at ball
              0, 1, 0);                                 // Up vector (Y is forward)
    
    glEnable(GL_DEPTH_TEST);
    
    // Enable lighting that follows the ball
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    
    // Spotlight directly above the ball  closer and focused
    float lightPos[] = {ballRenderPosition.x, ballRenderPosition.y, ballRenderPosition.z + 2.0f, 1.0f};  // Just 2 units above ball
    float lightDir[] = {0.0f, 0.0f, -2.0f};  // Point down
    float ambient[] = {0.3f, 0.2f, 0.15f, 1.0f};  
    float diffuse[] = {0.8f, 0.8f, 0.8f, 1.0f};   // Reduced brightness
    float specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightDir);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
    glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 50.0f);  // Wide enough for ball and ground around it
    glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 10.0f);  // Softer edges
    glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.8f);
    glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1f);
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.02f);
    
    // Draw ground 
    glColor3f(1.96f, 1.93f, 1.92f);
    glNormal3f(0.0f, 0.0f, 1.0f);  // Normal pointing up for lighting
    glBegin(GL_QUADS);
    float size = mazeDimension * 2.0f;
    glVertex3f(-size, -size, 0.0f);
    glVertex3f(size, -size, 0.0f);
    glVertex3f(size, size, 0.0f);
    glVertex3f(-size, size, 0.0f);
    glEnd();
    
    // Draw maze walls 
    for (int y = 0; y < mazeDimension; y++) {
        for (int x = 0; x < mazeDimension; x++) {
            if (maze->isWall(x, y)) {
                glPushMatrix();
                glTranslatef(x, y, 0.5f);
                
                // Draw main brown cube
                glColor3f(0.83f, 0.34f, 0.24f);  
                glutSolidCube(1.0);
                
                // Draw panel lines
                glColor3f(0.46f, 0.12f, 0.06f);  
                glLineWidth(2.5f);
                
                // Horizontal lines
                glBegin(GL_LINES);
                for (float z = -0.5f; z <= 0.5f; z += 0.33f) {
                    glVertex3f(-0.5f, -0.5f, z);
                    glVertex3f(0.5f, -0.5f, z);
                    
                    glVertex3f(-0.5f, 0.5f, z);
                    glVertex3f(0.5f, 0.5f, z);
                    
                    glVertex3f(-0.5f, -0.5f, z);
                    glVertex3f(-0.5f, 0.5f, z);
                    
                    glVertex3f(0.5f, -0.5f, z);
                    glVertex3f(0.5f, 0.5f, z);
                }
                
                // Vertical lines 
                for (float h = -0.5f; h <= 0.5f; h += 0.333f) {
                    glVertex3f(h, -0.5f, -0.5f);
                    glVertex3f(h, -0.5f, 0.5f);
                    
                    glVertex3f(h, 0.5f, -0.5f);
                    glVertex3f(h, 0.5f, 0.5f);
                    
                    glVertex3f(-0.5f, h, -0.5f);
                    glVertex3f(-0.5f, h, 0.5f);
                    
                    glVertex3f(0.5f, h, -0.5f);
                    glVertex3f(0.5f, h, 0.5f);
                }
                glEnd();
                
                glLineWidth(1.0f);
                glPopMatrix();
            }
        }
    }
    
    // Draw shadow under the ball
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glPushMatrix();
    glTranslatef(ballRenderPosition.x, ballRenderPosition.y, 0.01f);  // Just above ground
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(0.0f, 0.0f, 0.0f, 0.5f);  // Dark center
    glVertex3f(0.0f, 0.0f, 0.0f);  // Center
    glColor4f(0.0f, 0.0f, 0.0f, 0.0f);  // Fully transparent edges
    for (int i = 0; i <= 20; i++) {
        float angle = 2.0f * 3.14159f * i / 20.0f;
        glVertex3f(cos(angle) * 0.45f, sin(angle) * 0.45f, 0.0f);
    }
    glEnd();
    glPopMatrix();
    
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
    
    // Draw ball
    glPushMatrix();
    glTranslatef(ballRenderPosition.x, ballRenderPosition.y, ballRenderPosition.z);
    
    // Apply rotation for rolling effect
    glRotatef(glm::degrees(ballTotalRotation), 
              ballRotationAxis.x, ballRotationAxis.y, ballRotationAxis.z);
    
    // Setup material properties 
    float matAmbient[] = {0.1f, 0.1f, 0.12f, 1.0f};       
    float matDiffuse[] = {0.4f, 0.4f, 0.45f, 1.0f};      // Dark gray diffuse
    float matSpecular[] = {0.5f, 0.5f, 0.55f, 1.0f};     // Bright white specular 
    float matShininess[] = {20.0f};                     
    
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matAmbient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matDiffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matSpecular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, matShininess);
    
    // Main metallic sphere darker gunmetal gray
    glColor3f(0.4f, 0.4f, 0.45f);  // Dark gray color
    glutSolidSphere(0.25, 24, 24);  // More segments for smoother metal look
    
    // Draw dark grooves to show rotation 
    glColor3f(0.15f, 0.15f, 0.18f); 
    glPushMatrix();
    glRotatef(90, 0, 1, 0);
    glutWireSphere(0.26, 8, 8);
    glPopMatrix();
    
    glPushMatrix();
    glRotatef(90, 1, 0, 0);
    glutWireSphere(0.26, 8, 8);
    glPopMatrix();
    
    glPopMatrix();
    
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);  // Disable lighting for text rendering
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 800, 0, 600);
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    glColor3f(1.0f, 1.0f, 1.0f);  // White text
    stringstream ss;
    ss << "Level " << currentLevel << " | Ball: " << (int)ballRenderPosition.x << "," << (int)ballRenderPosition.y;
    glRasterPos2f(10, 570);
    for (char c : ss.str()) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
    
    // Show instructions when 'i' is pressed
    if (input->showInstructions) {
        // Draw semi-transparent black background for instructions
        glColor4f(0.0f, 0.0f, 0.0f, 0.85f);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glBegin(GL_QUADS);
            glVertex2f(150, 220);
            glVertex2f(650, 220);
            glVertex2f(650, 380);
            glVertex2f(150, 380);
        glEnd();
        glDisable(GL_BLEND);
        
        // instructions title 
        glColor3f(1.0f, 1.0f, 0.0f);
        glRasterPos2f(335, 350);
        string instructions1 = "HOW TO PLAY";
        for (char c : instructions1) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
        
        // instructions text 
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(240, 310);
        string instructions2 = "Use ARROW KEYS to move the ball";
        for (char c : instructions2) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
        
        glRasterPos2f(230, 275);
        string instructions3 = "Reach the exit to advance to next level";
        for (char c : instructions3) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
        }
        
        glRasterPos2f(337, 240);
        string instructions4 = "Press ESC to quit";
        for (char c : instructions4) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
    } else {
        glColor3f(1.0f, 1.0f, 1.0f);  // White text
        glRasterPos2f(10, 30);
        string help = "Press and hold I for instructions | ESC to quit";
        for (char c : help) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
    }
    
    glEnable(GL_LIGHTING);  // Re-enable lighting
    
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    
    glutSwapBuffers();
}

void Game::handleResize(int width, int height) {
    glViewport(0, 0, width, height);
    camera->aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}
