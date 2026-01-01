#include "Transformation.h"

Transformation::Transformation()
    : modelMatrix(1.0f) {}

void Transformation::update() {
    // does nothing — meant to be overridden
}

const glm::mat4& Transformation::getModelMatrix() const {
    return modelMatrix;
}