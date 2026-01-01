#pragma once
#include <glm/glm.hpp>

#include "ShaderProgram.h"

class ShaderProgram;
class Camera {
    std::vector<CameraObserver*> observers;
public:
    void setResolution(int width, int height);
    glm::ivec2 getResolution() const;

    Camera();
    // Mouse / input
    void processMouseMovement(float dx, float dy);// delty v pixelech
    //void processScroll(float yoffset); // optional - zoom
    //void setPosition(const glm::vec3& pos);
    //glm::vec3 getPosition() const;

    // aspect / viewport change
    void setAspectRatio(float aspect);
    void setShaderProgram(ShaderProgram* shader);


    glm::vec3 getFront() const;
    // access matrices (const refs)
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix() const;

    void processKeyboard(char direction, float deltaTime);
    glm::vec3 getPosition() const { return eye; }

    void update();

    void addObserver(CameraObserver* obs);
    void removeObserver(CameraObserver* obs);

    // Force recompute and notify observers
    //void recomputeAndNotify();

private:
    //void updateVectors(); // recompute target/look vector and view matrix
    //void notifyObservers();

    // camera parameters
    glm::vec3 eye;
    glm::vec3 up;
    glm::vec3 target;
    // spherical angles (alpha = polar, phi = azimuth)
    float alpha; // 0..pi  (polar)
    float phi;   // 0..2pi (azimuth)

    // projection params
    float fov;
    float aspect;
    float nearPlane;
    float farPlane;

    float yaw;   // rotation around Y axis
    float pitch; // rotation around X axis
    float speed;
    float sensitivity;
    float aspectRatio;

    glm::ivec2 resolution;

    // matrices cached
    //glm::mat4 viewMatrix;
    //glm::mat4 projectionMatrix;

    ShaderProgram* shaderProgram;

    void notifyObservers();

    // observers
    //std::vector<CameraObserver*> observers;

    // limits
    //const float alphaMin = 0.01f;
    //const float alphaMax = 3.13f; // slightly less than PI to avoid gimbal
};