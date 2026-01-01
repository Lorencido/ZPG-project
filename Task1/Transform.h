#pragma once
#include "Transformation.h"
#include <glm/glm.hpp>

class Transform : public Transformation {
    glm::vec3 translation;

public:
    Transform(const glm::vec3& t = glm::vec3(0.0f));

    void setTranslation(const glm::vec3& t);
    const glm::vec3& getTranslation() const { return translation; }
};