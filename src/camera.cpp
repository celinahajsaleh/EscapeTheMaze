#include "camera.h"

Camera::Camera(float fov, float aspect, float near, float far)
    : fov(fov), aspectRatio(aspect), nearPlane(near), farPlane(far), lerpFactor(0.1f) {
    position = glm::vec3(1.0f, 1.0f, 10.0f);
    target = glm::vec3(1.0f, 1.0f, 0.0f);
    up = glm::vec3(0.0f, 0.0f, 1.0f);
    desiredPosition = position;
}

void Camera::setTarget(const glm::vec3& targetPos, float targetZ) {
    desiredPosition = glm::vec3(targetPos.x, targetPos.y, targetZ);
}

void Camera::update() {
    position += (desiredPosition - position) * lerpFactor;
    target = glm::vec3(position.x, position.y, 0.0f);
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, target, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}
