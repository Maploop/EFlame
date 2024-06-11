#version 330 core

out vec4 FragColor;

in vec3 color;
in vec2 texCoord;
in vec3 Normal;
in vec3 currentPos;

uniform sampler2D tex00;
uniform sampler2D tex01;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main() {
    float ambient = 0.2;

    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - currentPos);
    float diffuse = max(dot(normal, lightDir), 0.0);

    float specularLight = 0.5;
    vec3 viewDir = normalize(camPos - currentPos);
    vec3 reflectionDir = reflect(-lightDir, normal);
    float specAmount = pow(max(dot(viewDir, reflectionDir), 0.0), 16);
    float specular = specularLight * specAmount;

    FragColor = texture(tex00, texCoord) * lightColor * (diffuse + ambient) + (texture(tex01, texCoord).r * specular);
}