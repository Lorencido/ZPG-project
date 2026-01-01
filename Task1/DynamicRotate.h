#pragma once
#include "Rotate.h"

class DynamicRotate : public Rotate {
public:
    DynamicRotate(float speedRad = 0.0f, glm::vec3 ax = glm::vec3(0, 0, 1));

    void setSpeed(float s);

    void update() override;

private:
    float speed;
};