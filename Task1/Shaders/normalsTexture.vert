#version 330 core
// input variables; from the vertex buffer, must match VBO attributes
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 textureUV;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

// output variables; will be passed to the fragment shader
out vec3 FragPos;
out vec3 Normal;
out vec2 Texture;

void main() {
    // position and normal in world space
    FragPos = (modelMatrix * vec4(position, 1.0)).xyz;
    Normal = mat3(transpose(inverse(modelMatrix))) * normal;
    Texture = textureUV;

    // vertex position in the clip space
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1.0);
}