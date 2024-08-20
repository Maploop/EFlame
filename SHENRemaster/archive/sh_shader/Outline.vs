#version 330 core

layout (location = 0) in vec3 aPos;
// Camera
uniform mat4 camMatrix;
// Transformations and matrices
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;
uniform float outlining;

void main() {
    vec3 crntPos = vec3(model * translation * -rotation * scale * outlining * vec4(aPos, 1.0));
    gl_Position = camMatrix * vec4(crntPos, 1.0);
}