#pragma once
#include "DrawableObject.h"
#include "Light.h"
#include "CompositeTransformation.h"
#include <glm/glm.hpp>

#include "Scale.h"

class Firefly {
    std::shared_ptr<DrawableObject> drawable;
    Light light;
    std::shared_ptr<CompositeTransformation> transform;
    glm::vec3 velocity;
    float boundary;
public:
    Firefly(Model* model, ShaderProgram* shader,
            const glm::vec3& startPos, const glm::vec3& color,
            const glm::vec3& velocity, float boundary = 10.0f)
    : light(LightType::Point, startPos, glm::vec3(0.0f, 0.0f, 0.0f), color, 1.0f),
          velocity(velocity), boundary(boundary)
    {
        auto scale = std::make_shared<Scale>(glm::vec3(0.05f));
        auto translate = std::make_shared<Transform>(startPos);
        transform = std::make_shared<CompositeTransformation>();
        transform->add(translate);
        transform->add(scale);


        drawable = std::make_shared<DrawableObject>(model, shader, transform.get(), color);
    }

    std::shared_ptr<DrawableObject> getDrawable() const { return drawable; }
    Light& getLight() { return light; }

    void update(float deltaTime) {
        glm::vec3 pos = light.position + velocity * deltaTime;


        for (int i = 0; i < 3; ++i) {
            if (pos[i] > boundary || pos[i] < -boundary) {
                velocity[i] *= -1.0f;
            }
        }

        light.position = pos;
        transform->setTranslation(pos);
    }
};
