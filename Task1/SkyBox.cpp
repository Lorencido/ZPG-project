#include "SkyBox.h"
#include <stb_image.h>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.h"

static float skyboxVertices[] = {
    -1,  1, -1,  -1, -1, -1,   1, -1, -1,
     1, -1, -1,   1,  1, -1,  -1,  1, -1,

    -1, -1,  1,  -1, -1, -1,  -1,  1, -1,
    -1,  1, -1,  -1,  1,  1,  -1, -1,  1,

     1, -1, -1,   1, -1,  1,   1,  1,  1,
     1,  1,  1,   1,  1, -1,   1, -1, -1,

    -1, -1,  1,  -1,  1,  1,   1,  1,  1,
     1,  1,  1,   1, -1,  1,  -1, -1,  1,

    -1,  1, -1,   1,  1, -1,   1,  1,  1,
     1,  1,  1,  -1,  1,  1,  -1,  1, -1,

    -1, -1, -1,  -1, -1,  1,   1, -1, -1,
     1, -1, -1,  -1, -1,  1,   1, -1,  1
};

SkyboxObject::SkyboxObject(const std::vector<std::string>& faces, ShaderProgram* shader)
    : DrawableObject(nullptr, shader, nullptr, glm::vec3(1,1,1))
{
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    stbi_set_flip_vertically_on_load(false);

    cubemapID = loadCubemap(faces);

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
}

GLuint SkyboxObject::loadCubemap(const std::vector<std::string>& faces)
{
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texID);

    int w, h, ch;
    for (int i = 0; i < 6; i++) {
        unsigned char* data = stbi_load(faces[i].c_str(), &w, &h, &ch, 3);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                         GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cerr << "Failed to load cubemap: " << faces[i] << "\n";
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texID;
}

void SkyboxObject::draw(const Camera& camera)
{
    glDepthFunc(GL_LEQUAL);

    shader->UseShader();

    glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix()));
    glm::mat4 projection = camera.getProjectionMatrix();

    shader->setUniformBool("isSkybox", true);
    shader->setUniformMatrix4fv("viewMatrix", glm::value_ptr(view));
    shader->setUniformMatrix4fv("projectionMatrix", glm::value_ptr(projection));

    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapID);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthFunc(GL_LESS);
}