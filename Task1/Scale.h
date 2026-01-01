#pragma once
#include "Transformation.h"
#include <glm/glm.hpp>

class Scale : public Transformation {
    glm::vec3 factors;

public:
    Scale(const glm::vec3& f = glm::vec3(1.0f));

    void setScale(const glm::vec3& f);
};