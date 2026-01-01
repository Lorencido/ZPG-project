#pragma once
#include <vector>
#include <GL/glew.h>
#include "Texture.h"

class Model {
    GLuint vao, vbo;
    int vertexCount;
    std::vector<Texture> textures;
public:
    Model(const char* name);
    Model(const float* data, size_t size, int count);
    void BindVAO() const;
    void DrawModel() const;
};