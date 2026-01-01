#version 330 core
in vec3 FragPos;
in vec3 Normal;
out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 ambientColor;
uniform float shininess;

void main()
{
    // --- Normalize inputs ---
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos); // from fragment to light
    vec3 viewDir  = normalize(viewPos - FragPos);  // from fragment to camera

    // --- Diffuse term ---
    float diff = max(dot(norm, lightDir), 0.0);

    // --- Blinn-Phong specular term ---
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);

    // --- Combine components ---
    vec3 ambient  = ambientColor * objectColor;
    vec3 diffuse  = diff * lightColor * objectColor;
    vec3 specular = spec * lightColor;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}