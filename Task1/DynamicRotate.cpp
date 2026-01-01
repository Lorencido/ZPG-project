#include "DynamicRotate.h"
#include <glm/gtc/matrix_transform.hpp>

DynamicRotate::DynamicRotate(float speedRad, glm::vec3 ax)
    : Rotate(speedRad, ax), speed(speedRad) {}

void DynamicRotate::setSpeed(float s) {
    speed = s;
}

void DynamicRotate::update() {
    angle += speed; // continuous spin
    modelMatrix = glm::rotate(glm::mat4(1.0f), angle, axis);
}