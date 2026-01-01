#pragma once
#include <vector>
#include <memory>

#include "Camera.h"
#include "DrawableObject.h"
#include "FireFlyObject.h"
#include "Light.h"
#include "SkyBox.h"


class Scene {
    std::vector<std::shared_ptr<DrawableObject>> objects;
    std::vector<Light> lights;
    std::vector<ShaderProgram*> shaders;
    std::vector<std::shared_ptr<FireFlyObject>> fireflies;
    std::shared_ptr<SkyboxObject> skybox;
public:
    int selectedID = -1;
    void add(const std::shared_ptr<DrawableObject>& obj);
    void remove(unsigned int id);

    void setSelected(unsigned int id);
    DrawableObject* getSelectedObject();
    int getSelectedID() const;

    void addFirefly(std::shared_ptr<FireFlyObject> f) {
        objects.push_back(f->getDrawable());
        fireflies.push_back(f);
    }

    void setSkybox(std::shared_ptr<SkyboxObject> sb);
    void setCamera(Camera* cam) { camera = cam; }
    Camera* getCamera() const { return camera; }

    std::vector<std::shared_ptr<FireFlyObject>>& getFireflies() { return fireflies; }

    void addLight(const Light& light) { lights.push_back(light); }


    //void update();
    void update(float deltaTime);
    void draw();

    const std::vector<std::shared_ptr<DrawableObject>>& getObjects() const { return objects; }
    std::vector<Light>& getLights() { return lights; }

    void addShader(ShaderProgram* shader) { shaders.push_back(shader); }
    std::vector<ShaderProgram*>& getShaders() { return shaders; }

    void updateLights(Camera& camera) {
        for (auto& light : lights) {
            if (light.getType() == LightType::Spotlight) {
                light.position  = camera.getPosition();
                light.direction = camera.getFront();
            }
        }
    }

    void activate() {

        for (auto* shader : shaders) {
            //shader->UseShader();

            shader->setInt("numLights", static_cast<int>(lights.size()));

            for (int i = 0; i < lights.size(); i++) {
                lights[i].applyToShader(*shader, i, lights.size());
            }

            for (int i = lights.size(); i < 20; i++) {
                std::string base = "lights[" + std::to_string(i) + "]";
                shader->setVec3((base + ".position").c_str(), glm::vec3(0.0f));
                shader->setVec3((base + ".color").c_str(), glm::vec3(0.0f));
                shader->setFloat((base + ".intensity").c_str(), 0.0f);
            }
        }
    }
private:
    Camera* camera = nullptr;
};
