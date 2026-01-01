#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "DrawableObject.h"
#include "Light.h"
#include "CompositeTransformation.h"

class FireFlyObject {
public:
    Light light;
    FireFlyObject(Model* model,
            ShaderProgram* shader,
            const glm::vec3& startPos,
            const glm::vec3& color,
            const glm::vec3& startVelocity = glm::vec3(0.1f, 0.1f, 0.1f),
            float scale = 0.05f,
            float boundary = 10.0f);

    void update(float deltaTime);
    void updateLight();

    std::shared_ptr<DrawableObject> getDrawable() const { return drawable; }


    //const Light& getLight() const { return light; }
    Light& getLightRef() { return light; }

private:
    std::shared_ptr<CompositeTransformation> transform; // shared ownership
    std::shared_ptr<DrawableObject> drawable;
    glm::vec3 velocity;
    float boundaryRadius;
    float scale;
    ShaderProgram* shader;
};
