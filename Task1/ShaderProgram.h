#pragma once
#include <GL/glew.h>
#include <vector>
#include "Shader.h"
#include <glm/glm.hpp>

class Shader;
class Camera;
class CameraObserver {
public:
    virtual void onCameraChanged(const glm::mat4& view, const glm::mat4& proj) = 0;
};
class ShaderProgram : public CameraObserver{
public:
    ShaderProgram(const std::vector<Shader>& shaders);
    ShaderProgram(const std::string& vertexPath, const std::string& fragmentPath);

    void UseShader() const;
    void attachCamera(Camera* cam);
    void setViewMatrix(const glm::mat4& view) const;
    void setProjectionMatrix(const glm::mat4& projection) const;

    GLuint getID() const { return programID; }
    void setUniformMatrix4fv(const char* name, const GLfloat* value) const;

    void onCameraChanged(const glm::mat4& view, const glm::mat4& proj) override;

    void setVec3(const std::string& name, const glm::vec3& value);
    void setFloat(const std::string& name, float value);
    void setInt(const std::string& name, int value);
    void setUniformBool(const std::string& name, bool value);


private:
    GLuint programID;

    Camera* camera;
};