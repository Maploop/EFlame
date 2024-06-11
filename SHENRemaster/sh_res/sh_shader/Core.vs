#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aNormal;

out vec3 color;
out vec2 texCoord;

out vec3 Normal;
out vec3 currentPos;

uniform mat4 camMatrix;
uniform mat4 model;

void main() {
    currentPos = vec3(model * vec4(aPos, 1.0));

    // gl_Position = vec4(aPos.x + (aPos.x * scale), aPos.y + (aPos.y * scale), aPos.z + (aPos.z * scale), 1.0);
    gl_Position = camMatrix * model * vec4(currentPos, 1.0f);
    color = aColor;
    texCoord = aTexCoords;
    Normal = aNormal;
}