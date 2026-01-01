#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform(const glm::vec3& t)
    : translation(t) {
    modelMatrix = glm::translate(glm::mat4(1.0f), translation);
}

void Transform::setTranslation(const glm::vec3& t) {
    translation = t;
    modelMatrix = glm::translate(glm::mat4(1.0f), translation);
}