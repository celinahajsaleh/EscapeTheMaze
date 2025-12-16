#ifndef PHYSICS_H
#define PHYSICS_H

#include <box2d/box2d.h>
#include <glm/glm.hpp>
#include <vector>

class Physics {
public:
    b2WorldId worldId;
    b2BodyId ballBodyId;
    float ballRadius;
    
    Physics();
    ~Physics();
    
    void createBall(float x, float y, float radius);
    void createWall(float x, float y, float width, float height);
    void applyForce(float x, float y);
    void update(float deltaTime);
    
    glm::vec2 getBallPosition() const;
    glm::vec2 getBallVelocity() const;
    void setBallPosition(float x, float y);
    
private:
    const float FRICTION_MULTIPLIER = 0.95f;
    const float FORCE_MULTIPLIER = 26.0f;
};

#endif
