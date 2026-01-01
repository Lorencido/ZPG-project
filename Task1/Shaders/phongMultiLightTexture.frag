#version 330 core
// input variables; from the vertex shader
in vec3 FragPos;
in vec3 Normal;
in vec2 Texture;
// output variable for fragment color
out vec4 FragColor;


#define MAX_LIGHTS 20 // Limit max count of lights in scene

uniform vec3 objectColor; // color of the object surface
uniform vec3 viewPos;
uniform int numLights;
uniform float shininess;
uniform sampler2D textureUnit;


struct Light {
    // 0=dir, 1=point, 2=spot
    int type;
    vec3 position;
    vec3 direction; // spot/directional light direction
    vec3 color; // color of the shining light
    float intensity;
    float constant; //attenuation coefficient; basic light intensity
    float linear; //attenuation coefficient; depends on the range of the light
    float quadratic; //attenuation coefficient; larger value ensures faster attenuation
    float cutOff;
    float outerCutOff;
};

uniform Light lights[MAX_LIGHTS];

vec3 CalcDirectionalLight(Light light, vec3 norm);
vec3 CalcPointLight(Light light, vec3 norm, vec3 FragPos);
vec3 CalcSpotLight(Light light, vec3 norm, vec3 FragPos);

void main() {
    vec3 textureColor = (texture(textureUnit, Texture)).rgb;
    vec3 norm = normalize(Normal);
    vec3 result = vec3(0.0);

    for (int i = 0; i < numLights; i++) {
        if (lights[i].type == 0)
        result += CalcDirectionalLight(lights[i], norm);
        else if (lights[i].type == 1)
        result += CalcPointLight(lights[i], norm, FragPos);
        else if (lights[i].type == 2)
        result += CalcSpotLight(lights[i], norm, FragPos);
    }

    //FragColor = vec4(result * objectColor, 1.0);
    FragColor = vec4(result * textureColor * objectColor, 1.0);
}

vec3 CalcDirectionalLight(Light light, vec3 norm)
{
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    //float spec = 0.0;

    //if (diff > 0.0) {
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
    //}

    vec3 ambient = 0.2 * light.color;
    vec3 diffuse = diff * light.color;
    vec3 specular = 0.5 * spec * light.color;

    return (ambient + diffuse + specular) * light.intensity;
}

vec3 CalcPointLight(Light light, vec3 norm, vec3 FragPos)
{
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    //float spec = 0.0;

    //if (diff > 0.0) {
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 1.0f);
    //}

    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = 0.2 * light.color;
    vec3 diffuse = diff * light.color;
    vec3 specular = 0.5 * spec * light.color;

    return (ambient + diffuse + specular) * attenuation * light.intensity;
}

vec3 CalcSpotLight(Light light, vec3 norm, vec3 FragPos)
{
    vec3 lightDir = normalize(light.position - FragPos);
    float theta = dot(lightDir, normalize(-light.direction));

    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float diff = max(dot(norm, lightDir), 0.0);
    //float spec = 0.0;

    //if (diff > 0.0) {
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
    //}

    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    vec3 ambient = 0.2 * light.color;
    vec3 diffuse = diff * light.color;
    vec3 specular = 0.5 * spec * light.color;

    return (ambient + diffuse + specular) * attenuation * intensity * light.intensity;
}
