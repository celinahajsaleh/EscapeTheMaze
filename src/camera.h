#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;
    
    float lerpFactor;
    
    Camera(float fov, float aspect, float near, float far);
    
    void setTarget(const glm::vec3& targetPos, float targetZ);
    void update();
    
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;
    
private:
    glm::vec3 desiredPosition;
};

#endif
