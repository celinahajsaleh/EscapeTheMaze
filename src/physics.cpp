#include "physics.h"

Physics::Physics() : ballRadius(0.25f) {
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = {0.0f, 0.0f};
    worldId = b2CreateWorld(&worldDef);
    ballBodyId = b2_nullBodyId;
}

Physics::~Physics() {
    b2DestroyWorld(worldId);
}

void Physics::createBall(float x, float y, float radius) {
    ballRadius = radius;
    
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = {x, y};
    bodyDef.linearDamping = 0.5f;
    
    ballBodyId = b2CreateBody(worldId, &bodyDef);
    
    b2Circle circle;
    circle.center = {0.0f, 0.0f};
    circle.radius = radius;
    
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 1.0f;
    
    b2CreateCircleShape(ballBodyId, &shapeDef, &circle);
}

void Physics::createWall(float x, float y, float width, float height) {
    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = b2_staticBody;
    bodyDef.position = {x, y};
    
    b2BodyId wallBodyId = b2CreateBody(worldId, &bodyDef);
    
    b2Polygon box = b2MakeBox(width / 2.0f, height / 2.0f);
    
    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = 0.0f;
    
    b2CreatePolygonShape(wallBodyId, &shapeDef, &box);
}

void Physics::applyForce(float x, float y) {
    if (B2_IS_NON_NULL(ballBodyId)) {
        float mass = b2Body_GetMass(ballBodyId);
        b2Vec2 force = {x * mass * FORCE_MULTIPLIER, y * mass * FORCE_MULTIPLIER};
        b2Body_ApplyForceToCenter(ballBodyId, force, true);
    }
}

void Physics::update(float deltaTime) {
    // Apply manual friction
    if (B2_IS_NON_NULL(ballBodyId)) {
        b2Vec2 velocity = b2Body_GetLinearVelocity(ballBodyId);
        velocity.x *= FRICTION_MULTIPLIER;
        velocity.y *= FRICTION_MULTIPLIER;
        b2Body_SetLinearVelocity(ballBodyId, velocity);
    }
    
    // Step the physics simulation
    b2World_Step(worldId, 1.0f / 60.0f, 4);
}

glm::vec2 Physics::getBallPosition() const {
    if (B2_IS_NON_NULL(ballBodyId)) {
        b2Vec2 pos = b2Body_GetPosition(ballBodyId);
        return glm::vec2(pos.x, pos.y);
    }
    return glm::vec2(0.0f, 0.0f);
}

glm::vec2 Physics::getBallVelocity() const {
    if (B2_IS_NON_NULL(ballBodyId)) {
        b2Vec2 vel = b2Body_GetLinearVelocity(ballBodyId);
        return glm::vec2(vel.x, vel.y);
    }
    return glm::vec2(0.0f, 0.0f);
}

void Physics::setBallPosition(float x, float y) {
    if (B2_IS_NON_NULL(ballBodyId)) {
        b2Body_SetTransform(ballBodyId, {x, y}, b2Rot_identity);
        b2Body_SetLinearVelocity(ballBodyId, {0.0f, 0.0f});
        b2Body_SetAngularVelocity(ballBodyId, 0.0f);
    }
}
