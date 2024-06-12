#version 330 core

out vec4 FragColor;

in vec3 currentPos;
in vec3 Normal;
in vec3 color;
in vec2 texCoord;

uniform sampler2D diffuse0;
uniform sampler2D specular0;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

uniform bool directionalLightEnabled;
uniform vec3 dirLightAngle;

vec4 pointLight()
{
    // NOTE this is for intensity over time, currently using the 1/ad^2 + bd + 1.0 equation 
    vec3 lightVector = lightPos - currentPos;
    float dist = length(lightVector);
    // TODO Make these two uniforms
    float a = 1.0; // quadratic
    float b = 0.04; // linear
    float intensity = 1.0 / (a * dist * dist + b * dist + 1.0);

    float ambient = 0.2;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightVector);
    float diffuse = max(dot(normal, lightDir), 0.0);

    float specularLight = 0.5;
    vec3 viewDir = normalize(camPos - currentPos);
    vec3 reflectionDir = reflect(-lightDir, normal);
    float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
    float specular = specularLight * specAmount;

    return (texture(diffuse0, texCoord) * (diffuse * intensity + ambient) + texture(specular0, texCoord).r * specular * intensity) * lightColor;
}

vec4 directionalLight()
{
    float ambient = 0.2;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(dirLightAngle);
    float diffuse = max(dot(normal, lightDir), 0.0);

    float specularLight = 0.5;
    vec3 viewDir = normalize(camPos - currentPos);
    vec3 reflectionDir = reflect(-lightDir, normal);
    float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
    float specular = specularLight * specAmount;

    return (texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor;
}

vec4 spotLight()
{
    float outerCone = 0.90;
    float innerCone = 0.95;

    float ambient = 0.2;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - currentPos);
    float diffuse = max(dot(normal, lightDir), 0.0);

    float specularLight = 0.5;
    vec3 viewDir = normalize(camPos - currentPos);
    vec3 reflectionDir = reflect(-lightDir, normal);
    float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
    float specular = specularLight * specAmount;

    float angle = dot(vec3(0.0, -1.0, 0.0), -lightDir);
    float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0, 1.0);

    return (texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor;
}

void main() {
    if (directionalLightEnabled) {
        FragColor = directionalLight() + pointLight();
    } else {
        FragColor = pointLight();
    }

    // FragColor = spotLight();
}