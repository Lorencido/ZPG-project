#include "Rotate.h"
#include <glm/gtc/matrix_transform.hpp>

Rotate::Rotate(float a, glm::vec3 ax)
    : angle(a), axis(ax) {
    modelMatrix = glm::rotate(glm::mat4(1.0f), angle, axis);
}

void Rotate::setAngle(float a) {
    angle = a;
    modelMatrix = glm::rotate(glm::mat4(1.0f), angle, axis);
}