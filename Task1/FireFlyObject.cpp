#include "FireFlyObject.h"
#include "Transform.h"
#include "Scale.h"
#include <glm/gtc/matrix_transform.hpp>
#include <cstdlib>

FireFlyObject::FireFlyObject(Model* model,
                 ShaderProgram* shader,
                 const glm::vec3& startPos,
                 const glm::vec3& color,
                 const glm::vec3& startVelocity,
                 float scale_,
                 float boundary)
    : transform(std::make_shared<CompositeTransformation>()),
      drawable(nullptr),
      light(LightType::Point, startPos, glm::vec3(0.0f, 0.0f, 0.0f), color, 3.0f),
      velocity(startVelocity),
      boundaryRadius(boundary),
      scale(scale_),
      shader(shader)

{

    auto sc = std::make_shared<Scale>(glm::vec3(scale));
    auto tr = std::make_shared<Transform>(startPos);


    transform->add(tr);
    transform->add(sc);


    drawable = std::make_shared<DrawableObject>(model, shader, transform.get(), color);

    light.constant = 1.0f;
    light.linear = 0.09f;
    light.quadratic = 0.032f;
    light.cutOff = glm::cos(glm::radians(12.5f));
    light.outerCutOff = glm::cos(glm::radians(17.5f));
    light.intensity = 5.0f;
    light.direction = glm::vec3(0.0f, 0.0f, 0.0f);
}

void FireFlyObject::update(float deltaTime) {

    glm::vec3 jitter(
        (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 0.5f,
        0.0f,
        (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 0.5f
    );

    velocity += jitter * deltaTime * 0.5f;

    float maxSpeed = 1.0f;
    if (glm::length(velocity) > maxSpeed)
        velocity = glm::normalize(velocity) * maxSpeed;

    glm::vec3 pos(0.0f);

    light.position += velocity * deltaTime;


    for (int i = 0; i < 3; ++i) {
        if (light.position[i] > boundaryRadius) {
            //light.position[i] = boundaryRadius;
            //velocity[i] *= -1.0f;
        } else if (light.position[i] < -boundaryRadius) {
            //light.position[i] = -boundaryRadius;
            //velocity[i] *= -1.0f;
        }
    }

    transform->setTranslation(light.position);


}
