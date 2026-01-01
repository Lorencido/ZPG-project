#pragma once
#include <glm/glm.hpp>
#include "ShaderProgram.h"

enum class LightType { Directional = 0, Point = 1, Spotlight = 2 };

class Light {
public:
    LightType type;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;

    // attenuation
    float constant;
    float linear;
    float quadratic;

    // spotlight params
    float cutOff;
    float outerCutOff;

    Light(LightType t, glm::vec3 pos, glm::vec3 dir, glm::vec3 col, float intens = 1.0f)
    : type(t), position(pos), direction(dir), color(col), intensity(intens),
      constant(1.0f), linear(0.09f), quadratic(0.032f),
      cutOff(glm::cos(glm::radians(12.5f))),
      outerCutOff(glm::cos(glm::radians(17.5f))) {}

    LightType getType() { return type; };

    void applyToShader(ShaderProgram& program, int index, int lightCount) const {
        program.UseShader();
        program.setInt("numLights", lightCount);
        std::string prefix = "lights[" + std::to_string(index) + "]";
        program.setInt(prefix + ".type", (int)type);
        program.setVec3(prefix + ".position", position);
        program.setVec3(prefix + ".direction", direction);
        program.setVec3(prefix + ".color", color);
        program.setFloat(prefix + ".intensity", intensity);
        program.setFloat(prefix + ".constant", 1.0f);
        program.setFloat(prefix + ".linear", 0.09f);
        program.setFloat(prefix + ".quadratic", 0.032f);
        program.setFloat(prefix + ".cutOff", glm::cos(glm::radians(12.5f)));
        program.setFloat(prefix + ".outerCutOff", glm::cos(glm::radians(15.0f)));
        program.setVec3("ambientColor", glm::vec3(0.2f, 0.2f, 0.2f));
        program.setFloat("shininess", 1.0f);
        program.setInt("textureUnit", 0);
        program.setInt("skybox", 1);
    }
};