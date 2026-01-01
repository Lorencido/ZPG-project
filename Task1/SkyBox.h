#pragma once
#include "DrawableObject.h"
#include <vector>
#include <string>

class SkyboxObject : public DrawableObject {
public:
    SkyboxObject(const std::vector<std::string>& faces, ShaderProgram* shader);

    virtual void draw(const Camera& camera);

    void setCamera(Camera* cam) { camera = cam; }

    virtual bool isSkybox() const { return true; }

private:
    GLuint loadCubemap(const std::vector<std::string>& faces);
    GLuint cubemapID;
    GLuint vao = 0, vbo = 0;
    Camera* camera = nullptr;
};