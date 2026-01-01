#version 330 core
// input variables; from the vertex shader
in vec3 FragPos;
in vec3 Normal;
// output variable for fragment color
out vec4 FragColor;

//uniform vec3 lightPos;
//uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 ambientColor;
uniform float shininess;

#define MAX_LIGHTS 4  // Limit max count of lights in scene

struct Light {
    vec3 position;
    vec3 color;
};

uniform int numLights;
uniform int renderMode;
uniform Light lights[MAX_LIGHTS];

void main() {
    // Normalize all directions
    vec3 result = vec3(0.0);
    vec3 norm = normalize(Normal);

    for (int i = 0; i < numLights; i++) {
        // vector from the light to the surface
        vec3 lightDir = normalize(lights[i].position - FragPos);

        // diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        float spec = 0.0;
        // only add specular if lit
        //if (diff > 0.0) {
            // specular
            vec3 viewDir = normalize(viewPos - FragPos); // vector from fragment to camera
            vec3 reflectDir = reflect(-lightDir, norm); // reflected light vector

            spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
        //}


        vec3 diffuse = diff * lights[i].color;

        vec3 specular = 0.5 * spec * lights[i].color;

        vec3 ambient = ambientColor * lights[i].color;

        result += (diffuse + specular + ambient);
    }

    FragColor = vec4(result * objectColor, 1.0);
}