#include "Model.h"

#include <iostream>
#include <string>

#ifndef TINYOBJLOADER_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION
#endif
#include "TextureLoader.h"
#include "tiny_obj_loader.h"

Model::Model(const char* name)
{
    std::string inputfile = std::string("assets/") + name;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    bool ret = tinyobj::LoadObj(
        &attrib,
        &shapes,
        &materials,
        &warn,
        &err,
        inputfile.c_str(),
        "assets/"     // base directory for MTL and textures
    );

    if (!warn.empty())
        std::cout << "TinyOBJ Warning: " << warn << std::endl;

    if (!err.empty())
        std::cerr << "TinyOBJ Error: " << err << std::endl;

    if (!ret)
        throw std::runtime_error("Failed to load OBJ file: " + inputfile);

    textures.clear();
    textures.reserve(materials.size());

    for (const auto& mat : materials)
    {
        if (!mat.diffuse_texname.empty()) {
            Texture tex;
            tex.path = "assets/" + mat.diffuse_texname;
            tex.id = TextureLoader::loadTexture(tex.path.c_str());
            textures.push_back(tex);
            std::cout << "Loaded texture: " << tex.path << "\n";
        }
        else {
            std::cout << "No texture\n";
        }
    }

    std::vector<float> vertices;
    vertices.reserve(shapes.size() * 3000); // Optional optimization

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {

            // Position
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

            // Normal
            if (index.normal_index >= 0) {
                vertices.push_back(attrib.normals[3 * index.normal_index + 0]);
                vertices.push_back(attrib.normals[3 * index.normal_index + 1]);
                vertices.push_back(attrib.normals[3 * index.normal_index + 2]);
            } else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }

            // UV
            if (index.texcoord_index >= 0) {
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 1]);
            } else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }
        }
    }

    vertexCount = vertices.size() / 8;  // 8 floats per vertex

    // ---- OpenGL buffers ----

    //GLuint VBO;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);

    // layout: position (0), normal (1), uv (2)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float),
                          (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float),
                          (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                          8 * sizeof(float),
                          (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}


Model::Model(const float* data, size_t size, int count): vertexCount(count)
{
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (GLvoid*)(3 * sizeof(float)));
}

void Model::BindVAO() const {
    glBindVertexArray(vao);
}

void Model::DrawModel() const {
    for (unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}