#include "CompositeTransformation.h"

void CompositeTransformation::add(const std::shared_ptr<Transformation>& t) {
    transformations.push_back(t);
}

void CompositeTransformation::update() {
    // Reset to identity
    modelMatrix = glm::mat4(1.0f);

    // Update and multiply all sub-transformations in order
    for (auto& t : transformations) {
        t->update();
        modelMatrix *= t->getModelMatrix();
    }
}

void CompositeTransformation::setTranslation(const glm::vec3& newPos) {
    for (auto& t : transformations) {
        if (auto transform = std::dynamic_pointer_cast<Transform>(t)) {
            transform->setTranslation(newPos);
            break;
        }
    }
}