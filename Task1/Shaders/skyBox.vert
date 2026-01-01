/*
#version 330 core
layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;


void main()
{
    TexCoords = aPos;

    vec4 pos = projectionMatrix * vec4(mat3(viewMatrix) * aPos, 1.0);
    gl_Position = pos.xyww;
} */

#version 330 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureUV;

uniform bool isSkybox;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec3 FragPos;
out vec3 Normal;
out vec3 TexCoords;
out vec2 Texture;

void main()
{
    if (isSkybox)
    {
        // SKYBOX BRANCH
        TexCoords = position;
        mat4 viewNoTranslate = mat4(mat3(viewMatrix));

        vec4 pos = projectionMatrix * viewNoTranslate * vec4(position, 1.0);
        gl_Position = pos.xyww;

        FragPos = vec3(0);
        Normal = vec3(0);
        Texture = vec2(0);
    }
    else
    {
        // NORMAL OBJECT BRANCH
        FragPos = vec3(modelMatrix * vec4(position, 1.0));
        Normal = mat3(transpose(inverse(modelMatrix))) * normal;
        Texture = textureUV;

        gl_Position = projectionMatrix * viewMatrix * vec4(FragPos, 1.0);

        TexCoords = vec3(0);
    }
}