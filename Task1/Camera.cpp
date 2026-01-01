#include "Camera.h"
#include "ShaderProgram.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>

Camera::Camera()
    : eye(0.0f, 0.0f, 1.5f),
      target(0.0f, 0.0f, -1.0f),
      yaw(-90.0f),
      pitch(0.0f),
      up(0.0f, 1.0f, 0.0f),
      fov(glm::radians(60.0f)), aspect(4.0f / 3.0f),
      nearPlane(0.1f), farPlane(100.0f),
      speed(2.5f),
      shaderProgram(nullptr)
{}

void Camera::addObserver(CameraObserver* obs) {
    if (obs) observers.push_back(obs);
    obs->onCameraChanged(getViewMatrix(), getProjectionMatrix());
}

void Camera::removeObserver(CameraObserver* obs) {
    observers.erase(std::remove(observers.begin(), observers.end(), obs), observers.end());
}

void Camera::notifyObservers() {
    glm::mat4 view = getViewMatrix();
    glm::mat4 proj = getProjectionMatrix();
    for (auto* obs : observers)
        obs->onCameraChanged(view, proj);
}

void Camera::setShaderProgram(ShaderProgram* shader) {
    shaderProgram = shader;
}

void Camera::setAspectRatio(float a) {
    aspect = a;
}

void Camera::update() {
    glm::mat4 view = getViewMatrix();
    glm::mat4 projection = getProjectionMatrix();

    if (shaderProgram) {
        shaderProgram->setViewMatrix(view);
        shaderProgram->setProjectionMatrix(projection);
    }
}

glm::vec3 Camera::getFront() const {
    return glm::normalize(target - eye);
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(eye, eye + target, up);
}

glm::mat4 Camera::getProjectionMatrix() const {
    return glm::perspective(fov, aspect, nearPlane, farPlane);
}

void Camera::setResolution(int width, int height) {
    resolution = { width, height };
}

glm::ivec2 Camera::getResolution() const {
    return resolution;
}


void Camera::processKeyboard(char direction, float deltaTime) {
    float velocity = speed * deltaTime;
    glm::vec3 right = glm::normalize(glm::cross(target, up));

    if (direction == 'W') eye += target * velocity;
    if (direction == 'S') eye -= target * velocity;
    if (direction == 'A') eye -= right * velocity;
    if (direction == 'D') eye += right * velocity;
}

void Camera::processMouseMovement(float dx, float dy) {
    const float sensitivity = 0.1f;
    dx *= sensitivity;
    dy *= sensitivity;

    yaw += dx;
    pitch -= dy;

    // Clamp vertical look
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    target = glm::normalize(front);
}