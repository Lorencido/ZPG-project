#pragma once
#include "Transformation.h"
#include <glm/glm.hpp>

class Rotate : public Transformation {
protected:
    float angle;
    glm::vec3 axis;

public:
    Rotate(float a = 0.0f, glm::vec3 ax = glm::vec3(0, 0, 1));

    void setAngle(float a);
};