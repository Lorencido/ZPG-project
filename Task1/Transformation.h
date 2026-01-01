#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transformation {
protected:
    glm::mat4 modelMatrix;
public:
    Transformation();
    virtual ~Transformation() = default;

    virtual void update();
    const glm::mat4& getModelMatrix() const;
};