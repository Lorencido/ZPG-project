#pragma once
#include "Model.h"
#include "ShaderProgram.h"
#include "Transformation.h"
#include "Camera.h"

class DrawableObject {
protected:
    Model* model;
    ShaderProgram* shader;
    Transformation* transform;
    glm::vec3 color;
public:
    unsigned int id;
    DrawableObject(Model* m, ShaderProgram* s, Transformation* t, glm::vec3 col);
    virtual void draw();
    void update() const;

    ShaderProgram* getShaderProgram() const { return shader; }
    unsigned int getID() const { return id; }
    void setID(unsigned int newID);
};