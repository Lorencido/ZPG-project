/*#version 330 core
out vec4 FragColor;
in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, TexCoords);
}
*/

#version 330 core

out vec4 FragColor;

// inputs from vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec2 Texture;
in vec3 TexCoords;

// mode switch
uniform bool isSkybox;

// samplers
uniform sampler2D textureUnit; // diffuse map for objects (bind to unit 0)
uniform samplerCube skybox;    // cubemap (bind to unit 1)

// lighting uniforms (only used when !isSkybox)
#define MAX_LIGHTS 20
uniform vec3 objectColor;
uniform vec3 viewPos;
uniform int numLights;
uniform float shininess;

struct Light {
    int type; // 0=dir,1=point,2=spot
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float constant;
    float linear;
    float quadratic;
    float cutOff;
    float outerCutOff;
};
uniform Light lights[MAX_LIGHTS];

// forward declarations
vec3 CalcDirectionalLight(Light light, vec3 norm);
vec3 CalcPointLight(Light light, vec3 norm, vec3 fragPos);
vec3 CalcSpotLight(Light light, vec3 norm, vec3 fragPos);

void main()
{
    if (isSkybox) {
        FragColor = texture(skybox, TexCoords);
        return;
    }

    // object rendering branch
    vec3 texColor = texture(textureUnit, Texture).rgb;
    vec3 norm = normalize(Normal);
    vec3 result = vec3(0.0);

    for (int i = 0; i < numLights; ++i) {
        if (lights[i].type == 0)
        result += CalcDirectionalLight(lights[i], norm);
        else if (lights[i].type == 1)
        result += CalcPointLight(lights[i], norm, FragPos);
        else if (lights[i].type == 2)
        result += CalcSpotLight(lights[i], norm, FragPos);
    }

    FragColor = vec4(result * texColor * objectColor, 1.0);
}


// ---------- lighting helpers ----------
vec3 CalcDirectionalLight(Light light, vec3 norm)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    vec3 ambient = 0.2 * light.color;
    vec3 diffuse = diff * light.color;
    vec3 specular = 0.5 * spec * light.color;
    return (ambient + diffuse + specular) * light.intensity;
}

vec3 CalcPointLight(Light light, vec3 norm, vec3 fragPos)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient = 0.2 * light.color;
    vec3 diffuse = diff * light.color;
    vec3 specular = 0.5 * spec * light.color;
    return (ambient + diffuse + specular) * attenuation * light.intensity;
}

vec3 CalcSpotLight(Light light, vec3 norm, vec3 fragPos)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float diff = max(dot(norm, lightDir), 0.0);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    vec3 ambient = 0.2 * light.color;
    vec3 diffuse = diff * light.color;
    vec3 specular = 0.5 * spec * light.color;
    return (ambient + diffuse + specular) * attenuation * intensity * light.intensity;
}