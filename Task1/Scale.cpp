#include "Scale.h"
#include <glm/gtc/matrix_transform.hpp>

Scale::Scale(const glm::vec3& f)
    : factors(f) {
    modelMatrix = glm::scale(glm::mat4(1.0f), factors);
}

void Scale::setScale(const glm::vec3& f) {
    factors = f;
    modelMatrix = glm::scale(glm::mat4(1.0f), factors);
}